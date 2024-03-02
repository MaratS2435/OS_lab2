#pragma once
#include <iostream>
#include <functional>
#include <map>

void AAA();
void BBB();
void CCC();
void DDD();
void EEE();
void FFF();
void GGG();
void HHH();

typedef std::function<void()> jobFunc;
extern const std::map<std::string, jobFunc> functionMap;