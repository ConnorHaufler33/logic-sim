#include "circuit.h"


static int evalGate(Gate *g, int signals[]) {
    int a = (g->inputA >= 0) ? signals[g->inputA] : 0;
    int b = (g->inputB >= 0) ? signals[g->inputB] : 0;
    int c = (g->inputC >= 0) ? signals[g->inputC] : 0;
    switch (g->type) {
        case GATE_HIGH: return 1;
        case GATE_LOW:  return 0;
        case GATE_AND:  return a && b;
        case GATE_OR:   return a || b;
        case GATE_NOT:  return !a;
        case GATE_NAND: return !(a && b);
        case GATE_NOR:  return !(a || b);
        case GATE_XOR:  return a ^ b;
        case GATE_OUTPUT: return a;
        case GATE_SWITCH:
            // Just return whatever is already stored in its output signal
            return signals[g->outputSignal];
        case GATE_HALF_ADDER:
            if (g->outputSignal >= 0) signals[g->outputSignal] = a ^ b;   // sum
            if (g->extraOutput >= 0) signals[g->extraOutput] = a & b;     // carry
            return signals[g->outputSignal];
        case GATE_FULL_ADDER:
        if(g->outputSignal >= 0) signals[g->outputSignal] = a ^ b ^ c;
        if(g->extraOutput >= 0) signals[g->extraOutput] = (a && b) || (a && c) || (b && c);
        return signals[g->outputSignal];
    }
    return 0;
}



void circuitEvaluate(Circuit *circuit){
    for (int pass = 0; pass < circuit->gateCount; pass++) {
        for (int i = 0; i < circuit->gateCount; i++) {
            Gate *gate = &circuit->gates[i];

            if (gate->type != GATE_OUTPUT && gate->outputSignal >= 0) {
                circuit->signals[gate->outputSignal] =
                    evalGate(gate, circuit->signals);
            }
        }
    }
}


void circuitRemoveGate(Circuit *circuit, int gateIndex){
    if (gateIndex < 0 || gateIndex >= circuit->gateCount) {
        return;
    }

    int deletedOutput =
        circuit->gates[gateIndex].outputSignal;

    int deletedExtraOutput =
        circuit->gates[gateIndex].extraOutput;

    int writeIndex = 0;

    /*
     * Remove connections going into the deleted gate or
     * coming from one of the deleted gate's outputs.
     */
    for (int i = 0; i < circuit->connectionCount; i++) {
        Connection connection = circuit->connections[i];

        int sourceWasDeleted =
            (deletedOutput >= 0 &&
             connection.fromSignalIndex == deletedOutput) ||
            (deletedExtraOutput >= 0 &&
             connection.fromSignalIndex == deletedExtraOutput);

        /*
         * Disconnect the destination gate's input when its
         * source gate is being deleted.
         */
        if (sourceWasDeleted &&
            connection.toGateIndex >= 0 &&
            connection.toGateIndex < circuit->gateCount) {

            Gate *destination =
                &circuit->gates[connection.toGateIndex];

            if (connection.toInputSlot == 0) {
                destination->inputA = -1;
            } else if (connection.toInputSlot == 1) {
                destination->inputB = -1;
            } else if (connection.toInputSlot == 2) {
                destination->inputC = -1;
            }
        }

        int shouldRemove =
            connection.toGateIndex == gateIndex ||
            sourceWasDeleted;

        if (!shouldRemove) {
            circuit->connections[writeIndex] = connection;
            writeIndex++;
        }
    }

    circuit->connectionCount = writeIndex;

    // Shift gates after processing their connections
    for (int i = gateIndex; i < circuit->gateCount - 1; i++) {
        circuit->gates[i] = circuit->gates[i + 1];
    }

    circuit->gateCount--;

    // Update connection indices affected by the shift
    for (int i = 0; i < circuit->connectionCount; i++) {
        if (circuit->connections[i].toGateIndex > gateIndex) {
            circuit->connections[i].toGateIndex--;
        }
    }

    circuitEvaluate(circuit);
}


int circuitAddGate(Circuit *circuit, GateType type){

    if (circuit->gateCount >= MAX_GATES) {
        return -1;
    }

    int signalsNeeded = 1;

    if (type == GATE_OUTPUT) {
        signalsNeeded = 0;
    } else if (type == GATE_HALF_ADDER ||
               type == GATE_FULL_ADDER) {
        signalsNeeded = 2;
    }

    if (circuit->nextSignalIndex + signalsNeeded > MAX_SIGNALS) {
        return -1;
    }

    int outputSignal = -1;
    int extraOutputSignal = -1;

    if (signalsNeeded >= 1) {
        outputSignal = circuit->nextSignalIndex++;
    }

    if (signalsNeeded == 2) {
        extraOutputSignal = circuit->nextSignalIndex++;
    }

    int gateIndex = circuit->gateCount++;

    circuit->gates[gateIndex] = (Gate){
        .type = type,
        .inputA = -1,
        .inputB = -1,
        .inputC = -1,
        .outputSignal = outputSignal,
        .extraOutput = extraOutputSignal
    };

    return gateIndex;
}


void circuitInit(Circuit *circuit)
{
    *circuit = (Circuit){0};

    circuit->nextSignalIndex = 2;
    circuit->signals[0] = 0;
    circuit->signals[1] = 1;
}


void circuitToggleSwitch(Circuit *circuit, int gateIndex)
{
    if (gateIndex < 0 || gateIndex >= circuit->gateCount) {
        return;
    }

    Gate *gate = &circuit->gates[gateIndex];

    if (gate->type != GATE_SWITCH ||
        gate->outputSignal < 0) {
        return;
    }

    circuit->signals[gate->outputSignal] ^= 1;
    circuitEvaluate(circuit);
}