#include <DCMotor.h>

DCMotor::DCMotor(const byte motorAPin, const byte motorBPin, const byte speedPin, const int direction, const bool active) :
                  motorAPin_(motorAPin), motorBPin_(motorBPin), speedPin_(speedPin)
  {
  active_ = active;
  direction_ = direction;
  }

void DCMotor::begin()
  {
  pinMode(motorAPin_, OUTPUT);
  pinMode(motorBPin_, OUTPUT);
  pinMode(speedPin_, OUTPUT);
  }

void DCMotor::move()
  {
  if (!active_)
    return;

  if (direction_)
    {
    digitalWrite(motorAPin_, LOW);
    digitalWrite(motorBPin_, HIGH);
    }
  else
    {
    digitalWrite(motorAPin_, HIGH);
    digitalWrite(motorBPin_, LOW);
    }
  analogWrite(speedPin_, 255);
  }

void DCMotor::start()
  {
  active_ = true;
  }

void DCMotor::stop()
  {
  active_ = false;
  digitalWrite(speedPin_, LOW);
  }

void DCMotor::changeDirection()
  {
  direction_ = !direction_;
  }

int DCMotor::getDirection() const
  {
  return direction_;
  }

void DCMotor::setDirection(int direction)
  {
  direction_ = direction;
  }

bool DCMotor::isMoving() const
  {
  return active_;
  }
