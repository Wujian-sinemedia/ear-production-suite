#pragma once

#include "JuceHeader.h"

#include <cmath>
#include <cstddef>
#include <mutex>
#include <vector>

namespace ear {
namespace plugin {

class LevelMeterCalculator {
 public:
  LevelMeterCalculator(std::size_t channels, std::size_t samplerate);

  /// change setup
  /**
   * Discards all measurements so far
   */
  void setup(std::size_t channels, std::size_t samplerate);

  /// Process input samples
  /**
   * Process all samples and update level values.
   *
   * @a input must have the correct channel count.
   */
  void process(const AudioBuffer<float>& buffer);

  /// process zeros if the last measurement is more than maxDuration (in
  /// milliseconds)
  void decayIfNeeded(int maxDuration);

  /// Get current level for a channel
  float getLevel(std::size_t channel);

  std::size_t samplerate() const { return samplerate_; }
  std::size_t channels() const { return channels_; }

 private:
  void processSample(float currentValue, std::size_t channel);

  void setConstants();
  void calcConstants();
  std::size_t channels_;
  std::size_t samplerate_;
  std::vector<float> lastLevel_;
  float release_constant_;
  float attack_constant_;
  int64_t lastMeasurement_;

  std::mutex mutex_;
};

}  // namespace plugin
}  // namespace ear
