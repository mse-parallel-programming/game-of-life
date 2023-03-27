package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-27
 */

@JsonInclude(JsonInclude.Include.NON_NULL)
public class UpdateAckMessage {
    public boolean next;

    public UpdateAckMessage(boolean next) {
        this.next = next;
    }
}
