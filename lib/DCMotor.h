#include <Arduino.h>

class DCMotor
  {
  public:

    DCMotor(const byte motorAPin, const byte motorBPin, const byte speedPin, const int direction = 0, const bool active = true);

    void begin();
    void move();
    void start();
    void stop();
    void changeDirection();
    int getDirection() const;
    void setDirection(int direction);
    bool isMoving() const;

  private:
    const byte motorAPin_;
    const byte motorBPin_;
    const byte speedPin_;

    int direction_;
    bool active_;

  };
