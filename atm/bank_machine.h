//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com
// Description : Bank state machine
//

#ifndef SHACK_BANK_MACHINE_H
#define SHACK_BANK_MACHINE_H

#include "withdraw.h"
#include "receiver.h"

class BankMachine {
 public:
  BankMachine() : balance_(199) {}

  void Done() {
    GetSender().Send(messaging::CloseQueue());
  }

  void Run() {
    try {
      while (true) {
        incoming_.Wait().Handle<VerifyPin>(
              [&](const VerifyPin &msg) {
                if (msg.pin_ == "1937") {
                  msg.atm_queue_.Send(PinVerified());
                } else {
                  msg.atm_queue_.Send(PinIncorrect()); 
                }
              } 
            ).Handle<Withdraw>(
              [&](const Withdraw &msg) {
                if (balance_ >= msg.amount_) {
                  msg.atm_queue_.Send(WithdrawOK());
                  balance_ -= msg.amount_; 
                } else {
                  msg.atm_queue_.Send(WithdrawDenied()); 
                }
              }  
            ).Handle<GetBalance>(
              [&](const GetBalance &msg) {
                msg.atm_queue_.Send(Balance(balance_)); 
              }  
            ).Handle<WithdrawProcessed>(
              [&](const WithdrawProcessed &msg) {}
            ).Handle<CancelWithdrawal>(
              [&](const CancelWithdrawal &msg) {}  
            );
      }
    } catch (const messaging::CloseQueue &) {}
  }

  messaging::Sender GetSender() {
    return incoming_;
  }
 private:
  messaging::Receiver incoming_;
  unsigned balance_;
};

#endif //SHACK_BANK_MACHINE_H
