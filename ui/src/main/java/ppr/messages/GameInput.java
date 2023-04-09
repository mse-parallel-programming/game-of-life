package ppr.messages;

import com.fasterxml.jackson.annotation.JsonInclude;

import java.util.List;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class GameInput {
    public int size;
    public List<List<Boolean>> grid;
}