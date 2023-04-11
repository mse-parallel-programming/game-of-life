package ppr.messages;

import java.util.List;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class GameInput {
    public int size;
    public List<List<Boolean>> grid;

    public int generations = 999;

    private boolean isManual = true;

    private boolean triggerNext = false;
    private boolean endProgram = false;
    public synchronized void setEnd() {
        this.endProgram = true;
        notifyAll(); // notify waiting threads that the value has changed
    }
    public  synchronized  boolean getEndProgram() {
        return this.endProgram;
    }
    public synchronized void setManual(boolean value) {
        this.isManual = value;
        notifyAll(); // notify waiting threads that the value has changed
    }

    public synchronized boolean getManual() {
        return this.isManual;
    }
    public synchronized void setNext(boolean value) {
        this.triggerNext = value;
        notifyAll(); // notify waiting threads that the value has changed
    }

    public synchronized boolean getNext() {
        return this.triggerNext;
    }

    public synchronized void waitUntilManualOrNextIsTrue() throws InterruptedException {
        while (this.isManual && !this.triggerNext && !endProgram) {
            wait(); // wait until flag is true
        }
    }
}