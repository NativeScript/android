/* Software Name : AsmDex
 * Version : 1.0
 *
 * Copyright © 2012 France Télécom
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.ow2.asmdex.util;

import java.util.List;

import org.ow2.asmdex.AnnotationVisitor;
import org.ow2.asmdex.MethodVisitor;
import org.ow2.asmdex.structureCommon.Label;

/**
 * Method Adapter used to shift all the registers found in the bytecode.
 *
 * @author Julien Névo
 */
public class RegisterShiftMethodAdapter extends MethodVisitor {

    private int nbRegistersToShift;

    /**
     * Constructor
     * @param api api level
     * @param mv method adapter
     * @param nbRegistersToShift offset of the shift.
     */
    public RegisterShiftMethodAdapter(int api, MethodVisitor mv, int nbRegistersToShift) {
        super(api, mv);
        this.nbRegistersToShift = nbRegistersToShift;
    }

    @Override
    public void visitMaxs(int maxStack, int maxLocals) {
        super.visitMaxs(maxStack + nbRegistersToShift, maxLocals);
    }

    @Override
    public void visitArrayLengthInsn(int destinationRegister, int arrayReferenceBearing) {
        super.visitArrayLengthInsn(destinationRegister + nbRegistersToShift,
                                   arrayReferenceBearing + nbRegistersToShift);
    }

    @Override
    public void visitFieldInsn(int opcode, String owner, String name,
                               String desc, int valueRegister, int objectRegister) {
        super.visitFieldInsn(opcode, owner, name, desc, valueRegister + nbRegistersToShift,
                             objectRegister + nbRegistersToShift);
    }

    @Override
    public void visitIntInsn(int opcode, int register) {
        super.visitIntInsn(opcode, register + nbRegistersToShift);
    }

    @Override
    public void visitJumpInsn(int opcode, Label label, int registerA,
                              int registerB) {
        super.visitJumpInsn(opcode, label, registerA + nbRegistersToShift, registerB + nbRegistersToShift);
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, Label end, int index) {
        super.visitLocalVariable(name, desc, signature, start, end, index + nbRegistersToShift);
    }

    @Override
    public void visitLocalVariable(String name, String desc, String signature,
                                   Label start, List<Label> ends, List<Label> restarts, int index) {
        super.visitLocalVariable(name, desc, signature, start, ends, restarts, index + nbRegistersToShift);
    }

    @Override
    public void visitFillArrayDataInsn(int arrayReference, Object[] arrayData) {
        super.visitFillArrayDataInsn(arrayReference + nbRegistersToShift, arrayData);
    }

    @Override
    public void visitArrayOperationInsn(int opcode, int valueRegister,
                                        int arrayRegister, int indexRegister) {
        super.visitArrayOperationInsn(opcode, valueRegister + nbRegistersToShift, arrayRegister + nbRegistersToShift,
                                      indexRegister + nbRegistersToShift);
    }

    @Override
    public void visitLookupSwitchInsn(int register, Label dflt, int[] keys,
                                      Label[] labels) {
        super.visitLookupSwitchInsn(register + nbRegistersToShift, dflt, keys, labels);
    }

    @Override
    public void visitTableSwitchInsn(int register, int min, int max,
                                     Label dflt, Label[] labels) {
        super.visitTableSwitchInsn(register + nbRegistersToShift, min, max, dflt, labels);
    }

    @Override
    public void visitMultiANewArrayInsn(String desc, int[] registers) {
        int[] registersCopy = cloneAndShift(registers, nbRegistersToShift);

        super.visitMultiANewArrayInsn(desc, registersCopy);
    }

    @Override
    public void visitMethodInsn(int opcode, String owner, String name,
                                String desc, int[] arguments) {
        super.visitMethodInsn(opcode, owner, name, desc, cloneAndShift(arguments, nbRegistersToShift));
    }

    @Override
    public void visitOperationInsn(int opcode, int destinationRegister,
                                   int firstSourceRegister, int secondSourceRegister, int value) {
        super.visitOperationInsn(opcode, destinationRegister + nbRegistersToShift,
                                 firstSourceRegister + nbRegistersToShift,
                                 secondSourceRegister + nbRegistersToShift, value);
    }

    @Override
    public AnnotationVisitor visitParameterAnnotation(int parameter,
            String desc, boolean visible) {
        return super.visitParameterAnnotation(parameter + nbRegistersToShift, desc, visible);
    }

    @Override
    public void visitStringInsn(int opcode, int destinationRegister,
                                String string) {
        super.visitStringInsn(opcode, destinationRegister + nbRegistersToShift, string);
    }

    @Override
    public void visitTypeInsn(int opcode, int destinationRegister,
                              int referenceBearingRegister, int sizeRegister, String type) {
        super.visitTypeInsn(opcode, destinationRegister + nbRegistersToShift, referenceBearingRegister + nbRegistersToShift,
                            sizeRegister + nbRegistersToShift, type);
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, int var) {
        super.visitVarInsn(opcode, destinationRegister + nbRegistersToShift, var);
    }

    @Override
    public void visitVarInsn(int opcode, int destinationRegister, long var) {
        super.visitVarInsn(opcode, destinationRegister + nbRegistersToShift, var);
    }

    /**
     * Returns a copy of the given array with every value shifted of the given number.
     * @param sourceArray the array to clone.
     * @param shift the number to add to every number of the source array.
     * @return an array composed of every value of the source array, shifted of the given number.
     */
    static private int[] cloneAndShift(int[] sourceArray, int shift) {
        int size = sourceArray.length;
        int[] destArray = new int[size];
        for (int i = 0; i < size; i++) {
            destArray[i] = sourceArray[i] + shift;
        }
        return destArray;
    }
}
