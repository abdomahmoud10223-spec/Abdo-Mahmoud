#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <string>

using namespace std;

// Forward Declarations
class CPU;
class Memory;

class ControlUnit {
public:
    ControlUnit();

    // فك شفرة التعليمات وتنفيذ العملية المناسبة
    void executeInstruction(const string& instruction, CPU& cpu, Memory& memory);

private:
    int hexToInt(const string& hexStr);
};

#endif
