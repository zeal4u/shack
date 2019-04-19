//
// Created by zeal4u on 2019/3/28.
// Author : zeal4u
// Email : jsz1995@live.com
// Description : Interface machine
//
#ifndef SHACK_INTERFACE_MACHINE_H
#define SHACK_INTERFACE_MACHINE_H

#include "receiver.h"
#include "withdraw.h"
#include <mutex>
#include <iostream>

class InterfaceMachine {
 public:
  void Done() {
    GetSender().Send(messaging::CloseQueue()); 
  }

  void Run() {
    try {
      while (true) {
        incoming_.Wait().Handle<IssueMoney>(
              [&](const IssueMoney &msg) {
                // 加锁以防止输出混乱
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Issuing " <<msg.amount_ <<std::endl;
              } 
            ).Handle<DisplayInsufficientFunds>(
              [&](const DisplayInsufficientFunds &) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Insufficient funds" <<std::endl;
              }  
            ).Handle<DisplayEnterPin>(
              [&](const DisplayEnterPin &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Please enter you PIN (0-9)" << std::endl;
              }
            ).Handle<DisplayEnterCard>(
              [&](const DisplayEnterCard &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Please enter your card (I)" << std::endl;
              }
            ).Handle<DisplayBalance>(
              [&](const DisplayBalance &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"The Balance of your account is " << msg.amount_ << std::endl;
              }
            ).Handle<DisplayWithdrawalOptions>(
              [&](const DisplayWithdrawalOptions &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Withdraw 50? (w)" << std::endl;
                std::cout<<"Display Balance? (b)" << std::endl;
                std::cout<<"Cancel? (c)" << std::endl;
              }
            ).Handle<DisplayWithdrawalCancelled>(
              [&](const DisplayWithdrawalCancelled &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Withdrawal cancelled" << std::endl;
              }
            ).Handle<DisplayPinIncorrectMessage>(
              [&](const DisplayPinIncorrectMessage &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"PIN incorrect" << std::endl;
              }
            ).Handle<EjectCard>(
              [&](const EjectCard &msg) {
                std::lock_guard<std::mutex> lk(io_mutex_);
                std::cout<<"Ejecting card" << std::endl;
              }
            );
      }
    } catch (messaging::CloseQueue &) {}
  }

  messaging::Sender GetSender() {
    return incoming_;
  }
 private:
  std::mutex io_mutex_;
  messaging::Receiver incoming_;
};

#endif// SHACK_INTERFACE_MACHINE_H
