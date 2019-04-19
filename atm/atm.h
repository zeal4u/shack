//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com
// Description : ATM state machine
//

#ifndef SHACK_ATM_H
#define SHACK_ATM_H

#include "sender.h"
#include "receiver.h"
#include "withdraw.h"
#include <string>

class ATM {
 public:
  ATM(messaging::Sender bank, messaging::Sender interface_hardware)
      : bank_(bank),
        interface_hardware_(interface_hardware) {}

  void Done() {
    GetSender().Send(messaging::CloseQueue());
  }

  void Run() {
    state_ = &ATM::WaitForCard;
    try {
      while (true) {
        (this->*state_)();
      }
    } catch (const messaging::CloseQueue &) {
      // TODO
    }
  }

  messaging::Sender GetSender() {
    return incoming_;
  }
 private:
  messaging::Receiver incoming_;
  messaging::Sender bank_;
  messaging::Sender interface_hardware_;
  void (ATM::*state_)();
  std::string account_;
  unsigned withdrawal_amount_;
  std::string pin_;

  void ProcessWithdrawal() {
    incoming_.Wait().Handle<WithdrawOK>(
          [&](const WithdrawOK &msg) {
            interface_hardware_.Send(IssueMoney(withdrawal_amount_));
            bank_.Send(WithdrawProcessed(account_, withdrawal_amount_));
            state_ = &ATM::DoneProcessing;
          }
        ).Handle<WithdrawDenied>(
          [&](const WithdrawDenied &msg) {
            interface_hardware_.Send(DisplayInsufficientFunds());
            state_ = &ATM::DoneProcessing;
          }
        ).Handle<CancelPressed>(
          [&](const CancelPressed &msg) {
            bank_.Send(CancelWithdrawal(account_, withdrawal_amount_));
            interface_hardware_.Send(DisplayWithdrawalCancelled());
            state_ = &ATM::DoneProcessing;
          }
        );
  }

  void ProcessBalance() {
    incoming_.Wait().Handle<Balance>(
          [&](const Balance &msg) {
            interface_hardware_.Send(DisplayBalance(msg.amount_));
            state_ = &ATM::WaitForAction;
          }
        ).Handle<CancelPressed>(
          [&](const CancelPressed &msg) {
            state_ = &ATM::DoneProcessing;
          }
        );
  }

  void WaitForAction() {
    interface_hardware_.Send(DisplayWithdrawalOptions());
    incoming_.Wait().Handle<WithdrawPressed>(
          [&](const WithdrawPressed &msg) {
            withdrawal_amount_ = msg.amount_;
            bank_.Send(Withdraw(account_, msg.amount_, incoming_));
            state_ = &ATM::ProcessWithdrawal;
          }
        ).Handle<BalancePressed>(
          [&](const BalancePressed &msg) {
            bank_.Send(GetBalance(account_, incoming_));
            state_ = &ATM::ProcessBalance;
          }
        ).Handle<CancelPressed>(
          [&](const CancelPressed &msg) {
            state_ = &ATM::DoneProcessing;
          }
        );
  }

  void VerifyingPin() {
    incoming_.Wait().Handle<PinVerified>(
          [&](const PinVerified &msg) {
            state_ = &ATM::WaitForAction;
          }
        ).Handle<PinIncorrect>(
          [&](const PinIncorrect &msg) {
            interface_hardware_.Send(DisplayPinIncorrectMessage());
            state_ = &ATM::DoneProcessing;
          }
        ).Handle<CancelPressed>(
          [&](const CancelPressed &msg) {
            state_ = &ATM::DoneProcessing;
          }
        );
  }

  void GettingPin() {
    incoming_.Wait().Handle<DigitPressed>(
          [&](const DigitPressed &msg) {
            const unsigned pin_length = 4;
            pin_ += msg.digit_;
            if (pin_length == pin_.size()) {
              bank_.Send(VerifyPin(account_, pin_, incoming_));
              state_ = &ATM::VerifyingPin;
            }
          }
        ).Handle<ClearLastPressed>(
          [&](const ClearLastPressed &msg) {
            if (!pin_.empty()) {
              pin_.pop_back();
            }
          }
        ).Handle<CancelPressed>(
          [&](const CancelPressed &msg) {
            state_ = &ATM::DoneProcessing;
          }
        );
  }

  void WaitForCard() {
    interface_hardware_.Send(DisplayEnterCard());
    incoming_.Wait().Handle<CardInserted>(
          [&](const CardInserted &msg) {
            account_ = msg.account_;
            pin_ = "";
            interface_hardware_.Send(DisplayEnterPin());
            state_ = &ATM::GettingPin;
          }
        );
  }
  void DoneProcessing() {
    interface_hardware_.Send(EjectCard());
    state_ = &ATM::WaitForCard;
  }
};

#endif //SHACK_ATM_H
