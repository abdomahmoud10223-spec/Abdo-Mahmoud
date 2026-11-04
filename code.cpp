#include "ControlUnit.h"
#include "CPU.h"
#include "Memory.h"
#include <iostream>
#include <sstream>

using namespace std;

ControlUnit::ControlUnit() {}

int ControlUnit::hexToInt(const string& hexStr) {
    int val = 0;
    stringstream ss;
    ss << hex << hexStr;
    ss >> val;
    return val;
}

void ControlUnit::executeInstruction(const string& instruction, CPU& cpu, Memory& memory) {
    // التأكد من أن الأمر مكتمل وطوله 4 خانات
    if (instruction.length() < 4 || instruction == "0000") {
        return;
    }

    // تفكيك الخانات الأربع: OpCode - R - S - T
    char opCode = instruction[0];
    int r = hexToInt(instruction.substr(1, 1)); // Register R
    int s = hexToInt(instruction.substr(2, 1)); // Register S / Operand X
    int t = hexToInt(instruction.substr(3, 1)); // Register T / Operand Y
    int xy = hexToInt(instruction.substr(2, 2)); // Memory Address XY

    switch (opCode) {
        case '1': // LOAD RXY: تحميل محتوى الذاكرة XY إلى المسجل R[span_2](start_span)[span_2](end_span)
            cpu.setRegister(r, memory.getCell(xy));
            break;

        case '2': // LOAD RXY: تحميل القيمة المباشرة XY إلى المسجل R[span_3](start_span)[span_3](end_span)
            cpu.setRegister(r, instruction.substr(2, 2));
            break;

        case '3': // STORE RXY: تخزين محتوى المسجل R في الذاكرة XY[span_4](start_span)[span_4](end_span)
            memory.setCell(xy, cpu.getRegister(r));
            if (xy == 0xFF) { // شاشة العرض المربوطة بالعنوان FF[span_5](start_span)[span_5](end_span)
                cpu.appendToScreen(cpu.getRegister(r));
            }
            break;

        case '4': // MOVE 40RS: نقل المحتوى من المسجل S إلى المسجل R[span_6](start_span)[span_6](end_span)
            cpu.setRegister(r, cpu.getRegister(t)); 
            break;

        case '5': // ADD RST: جمع two's complement بين S و T والناتج في R[span_7](start_span)[span_7](end_span)
            cpu.addTwosComplement(r, s, t);
            break;

        case '6': // ADD RST: جمع Floating point بين S و T والناتج في R[span_8](start_span)[span_8](end_span)
            cpu.addFloatingPoint(r, s, t);
            break;

        case '7': // OR RST[span_9](start_span)[span_9](end_span)
            cpu.bitwiseOR(r, s, t);
            break;

        case '8': // AND RST[span_10](start_span)[span_10](end_span)
            cpu.bitwiseAND(r, s, t);
            break;

        case '9': // XOR RST[span_11](start_span)[span_11](end_span)
            cpu.bitwiseXOR(r, s, t);
            break;

        case 'A': // ROTATE R0X: تدوير بتات المسجل R يميناً عدد X من المرات[span_12](start_span)[span_12](end_span)
            cpu.rotateRight(r, t); 
            break;

        case 'B': // JUMP RXY: القفز لعنوان XY لو R بيساوي R0[span_13](start_span)[span_13](end_span)
            if (cpu.getRegister(r) == cpu.getRegister(0)) {
                cpu.setPC(xy);
            }
            break;

        case 'C': // HALT[span_14](start_span)[span_14](end_span)
            cpu.setHalted(true);
            break;

        default:
            cout << "[Control Unit Error] Unknown OpCode: " << opCode << endl;
            cpu.setHalted(true);
            break;
    }
}
