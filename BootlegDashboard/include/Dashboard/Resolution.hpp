
#pragma once 

#include <WIR/Math.hpp>

namespace bootleg
{
    const glm::uvec2 baseline {1920, 1080};
    const glm::uvec2 full {1280, 720};
    const glm::uvec2 retro {640, 360};

    const glm::vec2 baseline_f {baseline.x, baseline.y};
    const glm::vec2 full_f {full.x, full.y};
    const glm::vec2 retro_f {retro.x, retro.y};

    const glm::vec2 scale_full{ full_f / baseline_f };
    const glm::vec2 scale_retro{ retro_f / baseline_f };
}