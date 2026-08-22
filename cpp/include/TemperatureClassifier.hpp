#pragma once

enum class TempStatus { Ok = 0, Warning, Critical };

/* Pure, stateless classification logic - kept separate from LED/GPIO
 * concerns so it can be unit tested in isolation. */
class TemperatureClassifier {
public:
  static constexpr float kWarningC = 85.0f;
  static constexpr float kCriticalHiC = 105.0f;
  static constexpr float kCriticalLoC = 5.0f;

  static TempStatus classify(float tempC);
};
