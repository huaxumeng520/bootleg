
#include "WindowManager.hpp"

#include <miral/application_info.h>
#include <miral/toolkit_event.h>
#include <miral/window_info.h>
#include <miral/window_manager_tools.h>

#include <linux/input.h>


bootleg::WindowManagerPolicy::WindowManagerPolicy(miral::WindowManagerTools const& tools) :
    CanonicalWindowManagerPolicy{tools}
{
}

bool bootleg::WindowManagerPolicy::handle_keyboard_event(MirKeyboardEvent const* event)
{
    auto const action = miral::toolkit::mir_keyboard_event_action(event);
    auto const scan_code = miral::toolkit::mir_keyboard_event_scan_code(event);
    auto const modifiers = miral::toolkit::mir_keyboard_event_modifiers(event) & modifier_mask;

    if (action == mir_keyboard_action_down &&
            modifiers == mir_input_event_modifier_alt &&
            scan_code == KEY_TAB)
    {
        tools.focus_next_application();

        return true;
    }
    else if (action == mir_keyboard_action_down &&
            modifiers == mir_input_event_modifier_alt &&
            scan_code == KEY_GRAVE)
    {
        tools.focus_next_within_application();

        return true;
    }
    else if (action == mir_keyboard_action_down &&
             modifiers == (mir_input_event_modifier_alt | mir_input_event_modifier_shift) &&
             scan_code == KEY_GRAVE)
    {
        tools.focus_prev_within_application();

        return true;
    }
    else if (action == mir_keyboard_action_down && scan_code == KEY_F4)
    {
        switch (modifiers & modifier_mask)
        {
        case mir_input_event_modifier_alt:
            tools.ask_client_to_close(tools.active_window());;
            return true;

        default:
            break;
        }
    }

    return false;
}

bool bootleg::WindowManagerPolicy::handle_touch_event(MirTouchEvent const* event)
{
    auto const count = miral::toolkit::mir_touch_event_point_count(event);

    long total_x = 0;
    long total_y = 0;

    for (auto i = 0U; i != count; ++i)
    {
        total_x += miral::toolkit::mir_touch_event_axis_value(event, i, mir_touch_axis_x);
        total_y += miral::toolkit::mir_touch_event_axis_value(event, i, mir_touch_axis_y);
    }

    miral::Point const cursor{total_x/count, total_y/count};

    tools.select_active_window(tools.window_at(cursor));

    return false;
}

bool bootleg::WindowManagerPolicy::handle_pointer_event(MirPointerEvent const* event)
{
    auto const action = miral::toolkit::mir_pointer_event_action(event);

    miral::Point const cursor{
        miral::toolkit::mir_pointer_event_axis_value(event, mir_pointer_axis_x),
        miral::toolkit::mir_pointer_event_axis_value(event, mir_pointer_axis_y)};

    if (action == mir_pointer_action_button_down)
    {
        tools.select_active_window(tools.window_at(cursor));
    }

    return false;
}

void bootleg::WindowManagerPolicy::advise_focus_gained(miral::WindowInfo const& info)
{
    CanonicalWindowManagerPolicy::advise_focus_gained(info);

    /*
    if (auto session = splash->session())
    {
        auto const& app_info = tools.info_for(session);

        for (auto const& s : app_info.windows())
            tools.raise_tree(s);
    }
    */
}

auto bootleg::WindowManagerPolicy::place_new_window(miral::ApplicationInfo const& app_info, miral::WindowSpecification const& request) -> miral::WindowSpecification
{
    miral::WindowSpecification specification = CanonicalWindowManagerPolicy::place_new_window(app_info, request);

    if ((specification.type() == mir_window_type_normal || specification.type() == mir_window_type_freestyle) &&
        (!specification.parent().is_set() || !specification.parent().value().lock()))
    {
        specification.state() = mir_window_state_maximized;
        specification.size() = mir::optional_value<miral::Size>{}; // Ignore requested size (if any) when we maximize
        tools.place_and_size_for_state(specification, miral::WindowInfo{});

        if (!request.state().is_set() || request.state().value() != mir_window_state_restored)
            specification.state() = request.state();
    }

    return specification;
}

void bootleg::WindowManagerPolicy::handle_modify_window(miral::WindowInfo& window_info, miral::WindowSpecification const& modifications)
{
    miral::WindowSpecification specification = modifications;

    if ((window_info.type() == mir_window_type_normal || window_info.type() == mir_window_type_freestyle) &&
        !window_info.parent())
    {
        specification.state() = mir_window_state_maximized;
        specification.size() = mir::optional_value<miral::Size>{}; // Ignore requested size (if any) when we maximize
        tools.place_and_size_for_state(specification, window_info);

        if (!modifications.state().is_set() || modifications.state().value() != mir_window_state_restored)
            specification.state() = modifications.state();
    }

    CanonicalWindowManagerPolicy::handle_modify_window(window_info, specification);
}

void bootleg::WindowManagerPolicy::handle_request_drag_and_drop(miral::WindowInfo& /*window_info*/)
{
}

void bootleg::WindowManagerPolicy::handle_request_move(miral::WindowInfo& /*window_info*/, MirInputEvent const* /*input_event*/)
{
}

void bootleg::WindowManagerPolicy::handle_request_resize(miral::WindowInfo& /*window_info*/, MirInputEvent const* /*input_event*/, MirResizeEdge /*edge*/)
{
}

mir::geometry::Rectangle bootleg::WindowManagerPolicy::confirm_placement_on_display(miral::WindowInfo const& /*window_info*/, MirWindowState /*new_state*/,
    mir::geometry::Rectangle const& new_placement)
{
    return new_placement;
}
