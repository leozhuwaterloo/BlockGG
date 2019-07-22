#include "./includes/TempSound.hpp"
#include "../managers/includes/GameManager.hpp"

TempSound::TempSound(const glm::vec3 &pos, const ALuint &sound,
  const float &pitch, const float &gain)
  : GameObject(ObjectType::TempSound, pos)
{
  alGenSources(1, &sound_source);
  alSourcef(sound_source, AL_PITCH, pitch);
  alSourcef(sound_source, AL_GAIN, gain * resource_manager->game->command_manager->sound_effect);
  alSourcei(sound_source, AL_LOOPING, AL_FALSE);
  alSourcei(sound_source, AL_BUFFER, sound);
  alSource3f(sound_source, AL_POSITION, pos.x, pos.y, pos.z);
  alSourcePlay(sound_source);
}

TempSound::~TempSound() {
  alDeleteSources(1, &sound_source);
}

void TempSound::tick() {
  elapsed_time += resource_manager->game->time_delta;
  if (elapsed_time > 100.0f) {
    ALenum state;
    alGetSourcei(sound_source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
      resource_manager->game->remove(this);
    } else {
      elapsed_time -= 100.0f;
    }
  }
}
