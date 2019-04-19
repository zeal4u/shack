//
// Created by zeal4u on 2019/3/29.
// Author : zeal4u
// Email : jsz1995@live.com
//

#include "bank_machine.h"
#include "interface_machine.h"
#include "atm.h"
#include <thread>
#include <cstdio>
#include <iostream>

int main() {
  BankMachine bank;
  InterfaceMachine interface_hardware;
  ATM machine(bank.GetSender(), interface_hardware.GetSender());
  std::thread bank_thread(&BankMachine::Run, &bank);
  std::thread if_thread(&InterfaceMachine::Run, &interface_hardware);
  std::thread atm_thread(&ATM::Run, &machine);

  messaging::Sender atm_queue(machine.GetSender());

  bool quit_pressed = false;
  char c = '\0';
  while (!quit_pressed) {
    // discard '\n';
    do {
      c = getchar();
    } while (c == '\n');
    switch (c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        atm_queue.Send(DigitPressed(c));
        break;
      case 'b':
        atm_queue.Send(BalancePressed());
        break;
      case 'w':
        atm_queue.Send(WithdrawPressed(50));
        break;
      case 'c':
        atm_queue.Send(CancelPressed());
        break;
      case 'i':
        atm_queue.Send(CardInserted("love weijing"));
        break;
      case 'q':
        quit_pressed = true;
        break;
      default:
        std::cout<<"Unsupported operation!" << std::endl;
        break;
    }
  }
  bank.Done();
  machine.Done();
  interface_hardware.Done();
  atm_thread.join();
  bank_thread.join();
  if_thread.join();
  return 0;
}

