//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com
// Description : ATM messages
//

#ifndef SHACK_WITHDRAW_H
#define SHACK_WITHDRAW_H

#include "sender.h"
#include <string>

struct Withdraw {
  std::string account_;
  unsigned amount_;
  mutable messaging::Sender atm_queue_;

  Withdraw(const std::string &account, unsigned amount, messaging::Sender atm_queue)
      : account_(account),
        amount_(amount),
        atm_queue_(atm_queue) {}
};

struct WithdrawOK {};

struct WithdrawDenied {};

struct CancelWithdrawal {
  std::string account_;
  unsigned amount_;

  CancelWithdrawal(const std::string &account, unsigned amount)
      : account_(account), amount_(amount) {}
};

struct WithdrawProcessed {
  std::string account_;
  unsigned amount_;

  WithdrawProcessed(const std::string account, unsigned amount)
      : account_(account),
        amount_(amount) {}
};

struct CardInserted {
  std::string account_;
  explicit CardInserted(const std::string account) : account_(account) {}
};

struct DigitPressed {
  char digit_;
  explicit DigitPressed(char digit) : digit_(digit) {}
};

struct ClearLastPressed {};
struct EjectCard {};

struct WithdrawPressed {
  unsigned amount_;
  explicit WithdrawPressed(unsigned amount) : amount_(amount) {}
};

struct CancelPressed {};

struct IssueMoney {
  unsigned amount_;
  IssueMoney(unsigned amount) : amount_(amount) {}
};

struct VerifyPin {
  std::string account_;
  std::string pin_;
  mutable messaging::Sender atm_queue_;
  VerifyPin(const std::string account, const std::string &pin, messaging::Sender atm_queue)
      : account_(account),
        pin_(pin),
        atm_queue_(atm_queue) {}
};

struct PinVerified {};
struct PinIncorrect {};
struct DisplayEnterPin {};
struct DisplayEnterCard {};
struct DisplayInsufficientFunds {};
struct DisplayWithdrawalCancelled {};
struct DisplayPinIncorrectMessage {};
struct DisplayWithdrawalOptions {};

struct GetBalance {
  std::string account_;
  mutable messaging::Sender atm_queue_;

  GetBalance(const std::string &account, messaging::Sender atm_queue)
      : account_(account),
        atm_queue_(atm_queue) {}
};

struct Balance {
  unsigned amount_;
  explicit Balance(unsigned amount) : amount_(amount) {}
};

struct DisplayBalance {
  unsigned amount_;
  explicit DisplayBalance(unsigned amount) : amount_(amount) {}
};

struct BalancePressed {};

#endif //SHACK_WITHDRAW_H
