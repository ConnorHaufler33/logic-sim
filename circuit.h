// circuit.h
#ifndef CIRCUIT_H
#define CIRCUIT_H

#define MAX_GATES 100
#define MAX_SIGNALS 200
#define MAX_CONNECTIONS 100

typedef enum {
    GATE_AND,
    GATE_OR,
    GATE_NOT,
    GATE_NAND,
    GATE_NOR,
    GATE_XOR,
    GATE_HIGH,
    GATE_LOW,
    GATE_OUTPUT,
    GATE_SWITCH,
    GATE_HALF_ADDER,
    GATE_FULL_ADDER
} GateType;

typedef struct {
    GateType type;
    int inputA;
    int inputB;
    int inputC;
    int outputSignal;
    int extraOutput;
} Gate;

typedef struct {
    int fromSignalIndex;
    int toGateIndex;
    int toInputSlot;
} Connection;

typedef struct {
    Gate gates[MAX_GATES];
    Connection connections[MAX_CONNECTIONS];
    int signals[MAX_SIGNALS];

    int gateCount;
    int connectionCount;
    int nextSignalIndex;
} Circuit;

void circuitInit(Circuit *circuit);
int circuitAddGate(Circuit *circuit, GateType type);
void circuitRemoveGate(Circuit *circuit, int gateIndex);
void circuitToggleSwitch(Circuit *circuit, int gateIndex);
void circuitEvaluate(Circuit *circuit);

#endif