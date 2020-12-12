
#include <Odin/Instance.hpp>
#include <Odin/Renderpass.hpp>
#include <Odin/Subpass.hpp>
#include <Odin/RenderTarget.hpp>
#include <Odin/DataBuffer.hpp>
#include <Odin/Sampler.hpp>
#include <Odin/Shader.hpp>
#include <Odin/Program.hpp>
#include <Odin/CommandBuffer.hpp>
#include <Odin/Fence.hpp>
#include <Odin/Parameters.hpp>
#include <Odin/RenderDisplay.hpp>
#include <Odin/ThreadContext.hpp>

#include <WIR/Math.hpp>
#include <WIR/Timer.hpp>
#include <WIR/Error.hpp>

#include <fstream>
#include <sstream>
#include <string>

namespace res
{

  const char *vertexSource = R"SHDR(
#version 450 core

in vec4 vertexPosition;
in vec2 vertexUv;

out vec2 fragmentUv;

layout(binding=0) uniform parameters
{
    mat4 mvpMatrix;
    float seconds;
};

void main()
{
    gl_Position = mvpMatrix * vertexPosition;
    fragmentUv = vertexUv;
}
)SHDR";

  const char *fragmentSource = R"SHDR(
#version 450 core

in vec2 fragmentUv;
out vec4 outColor;

layout(binding=0) uniform parameters
{
    mat4 mvpMatrix;
    float seconds;
};

layout(binding=1) uniform sampler2D albedo;

void main()
{
    outColor = texture(albedo, fragmentUv);
}

)SHDR";

  const uint32_t vertexCount = 8;
  const uint32_t vertexLength = 6 * vertexCount;
  const uint32_t vertexSize = vertexLength * sizeof(float);

  /*
    6   7

    4   5

  2   3

  0   1
*/

  const float vertexData[vertexLength] = {
      //  X     Y     Z     W     U     V
      -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

      -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f};

  const uint32_t indexLength = 6 * 6;
  const uint32_t indexSize = indexLength * sizeof(uint32_t);

  const uint32_t indexData[6 * 6] = {

      // Front
      2, 1, 0,
      2, 3, 1,

      // Right
      3, 5, 1,
      3, 7, 5,

      // Back
      7, 4, 5,
      7, 6, 4,

      // Left
      6, 0, 4,
      6, 2, 0,

      // Top
      6, 3, 2,
      6, 7, 3,

      // Bottom
      5, 0, 1,
      5, 4, 0

  };

  const glm::uvec2 albedoResolution = {4, 4};

  const uint32_t albedoSize = 4 * 4 * 4;

  const uint8_t albedoData[albedoSize] = {
    0,   0, 0, 255,
    85,  0, 0, 255,
    170, 0, 0, 255,
    255, 0, 0, 255,

    0,   85, 0, 255,
    85,  85, 0, 255,
    170, 85, 0, 255,
    255, 85, 0, 255,

    0,   170, 0, 255,
    85,  170, 0, 255,
    170, 170, 0, 255,
    255, 170, 0, 255,

    0,   255, 0, 255,
    85,  255, 0, 255,
    170, 255, 0, 255,
    255, 255, 0, 255,
  };

  odin::Texture *albedo = nullptr;
  odin::Sampler *albedoSampler = nullptr;

  odin::DataBuffer *vertexBuffer = nullptr;
  odin::DataBuffer *indexBuffer = nullptr;

  odin::Shader *vertexShader = nullptr;
  odin::Shader *fragmentShader = nullptr;

  void load(odin::Context *ctx)
  {
    vertexBuffer = new odin::DataBuffer(ctx, vertexSize, odin::BF_Vertex);
    vertexBuffer->uploadData(reinterpret_cast<uint8_t const*>(vertexData), vertexSize);

    indexBuffer = new odin::DataBuffer(ctx, indexSize, odin::BF_Index);
    indexBuffer->uploadData(reinterpret_cast<uint8_t const *>(indexData), indexSize);

    vertexShader = new odin::Shader(ctx, odin::SS_Vertex, vertexSource, {}, true);
    fragmentShader = new odin::Shader(ctx, odin::SS_Fragment, fragmentSource, {}, true);

    albedo = new odin::Texture(ctx, albedoResolution, odin::F_RGBA8_SRGB, 1);
    albedo->uploadData(reinterpret_cast<uint8_t const *>(albedoData), albedoSize);

    albedoSampler = new odin::Sampler(ctx, odin::F_Nearest, odin::ES_Clamp);
  }

  void unload()
  {
    delete albedoSampler;
    delete albedo;
    delete vertexShader;
    delete fragmentShader;
    delete vertexBuffer;
    delete indexBuffer;
  }

} // namespace res

int main(int argc, char **argv)
{
  wir::Timer perfTimer;
  wir::Timer frameTimer;

  odin::RenderDisplayParams displayParams;
  displayParams.resolution = glm::uvec2(1280, 720);
  displayParams.maxConcurrentFrames = 2;
  displayParams.developmentSurface = argc <= 1;
  auto renderDisplay = new odin::RenderDisplay(displayParams);

  auto instance = new odin::Instance("MyAppName", renderDisplay);
  auto threadContext = new odin::ThreadContext(instance);

  renderDisplay->initialize(threadContext);

  


  // Load resources
  res::load(instance);

  // Create default shaderprogram and parameters for it
  odin::PipelineSettings settings;
  auto program = new odin::Program(instance, {res::vertexShader, res::fragmentShader}, settings);
  program->link();

  glm::mat4 viewMatrix = glm::mat4(1.0f);
  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 4.0f));
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(72.f), 1280.f / 720.f, 0.01f, 100.0f);
  glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

  auto params = new odin::Parameters(threadContext, program);
  params->set("parameters", "mvpMatrix", mvpMatrix);
  params->set("parameters", "seconds", 0.0f);
  params->set("albedo", res::albedo, res::albedoSampler);

  // Create simple renderpass with one color attachemnt, one depth attachment and a single subpass
  auto renderpass = new odin::RenderPass(instance);
  auto renderpassOut = renderpass->createAttachment(odin::F_RGBA16_SFLOAT);
  auto renderpassDepth = renderpass->createAttachment(odin::F_D32_SFLOAT);
  auto renderpassLdr = renderpass->createAttachment(odin::F_RGBA8_SRGB);
  auto subpass = renderpass->createSubpass({renderpassOut}, renderpassDepth);
  auto subpassTonemap = renderpass->createSubpass({renderpassLdr}, nullptr, {renderpassOut});
  renderpass->link();

  // Create a rendertarget for our renderpass, and set the clearvalue to blue
  auto rendertarget = new odin::RenderTarget(renderpass, glm::uvec2(1280, 720));
  rendertarget->clearValue(renderpassOut, glm::vec4(0.05f, 0.2f, 0.7f, 1.0f));

  // Get the output texture from the rendertarget, and set it as the source for the renderwindow
  auto output = rendertarget->texture(renderpassOut);
  renderDisplay->source(output);

  // Assemble a renderpass command
  odin::ProgramCommand progCmd;
  progCmd.program = program;
  progCmd.parameters = params;
  progCmd.indexBuffer = res::indexBuffer;
  progCmd.indexCount = res::indexLength;
  progCmd.vertexBuffer = res::vertexBuffer;
  progCmd.vertexCount = res::vertexCount;

  odin::SubpassCommand subCmd;
  subCmd.subpass = subpass;
  subCmd.programs = {progCmd};

  odin::RenderPassCommand cmd;
  cmd.renderPass = renderpass;
  cmd.renderTarget = rendertarget;
  cmd.subpasses = {subCmd};

  // Create a command buffer and record the command to it
  auto cmdBuffer = new odin::CommandBuffer(threadContext);
  cmdBuffer->record({cmd});

  // Create a frame fence

  auto fence = new odin::Fence(instance);

  auto startupMs = perfTimer.seconds() * 1000.f;
  LogNotice("Startup time: %f ms", startupMs);
  perfTimer.reset();

  frameTimer.reset();
  float time = 0.0f;
  uint32_t frames = 0;

  while (!renderDisplay->wantsClose())
  {
    auto deltaTime = frameTimer.seconds();
    frameTimer.reset();
    time += deltaTime;

    renderDisplay->update(deltaTime);

    
    modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f * float(deltaTime)), glm::vec3(1.0f, 1.0f, 0.0f));
    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    params->set("parameters", "mvpMatrix", mvpMatrix);

    
    if (fence->signaled())
    {
      fence->reset();

      cmdBuffer->submit(fence);
    

      renderDisplay->present();
      frames++;
    }




    if (perfTimer.seconds() >= 1.0f)
    {
      LogNotice("FPS: %u", frames);
      renderDisplay->title(wir::format("FPS: %u", frames));
      frames = 0;
      perfTimer.reset();
    }
  }

  delete fence;
  delete cmdBuffer;
  delete rendertarget;
  delete renderpass;
  delete params;
  delete program;
  res::unload();


  renderDisplay->destroy();
  delete threadContext;
  delete instance;

  delete renderDisplay;
  return 0;
}
