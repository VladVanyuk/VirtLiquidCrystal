#ifndef SAFESTATE_H
#define SAFESTATE_H

#include <Arduino.h>
#include <string>

class SafeState {
  public:
    SafeState();
    void lock();
    bool unlock(String code);
    bool locked();
    bool hasCode();
    void setCode(String newCode);

  private:
    void setLock(bool locked);
    bool _locked;
};

#endif /* SAFESTATE_H */
