package com.tns;

/**
 * Created by pkanev on 8/30/2016.
 */
public class MessageType {
    public static int Handshake = 0;
    public static int MainToWorker = 1;
    public static int WorkerToMain = 2;
    public static int TerminateThread = 4;
    public static int CloseWorker = 6;
    public static int BubbleUpException = 7;
    public static int TerminateAndCloseThread = 8;
}
