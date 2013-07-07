package metrics.tokenisers;

import java.util.ArrayList;
import java.util.Set;

import metrics.wordhandlers.InterfaceTermHandler;

public interface InterfaceTokeniser {

    public String getShortDescriptionString();

    public String getDelimiters();

    public InterfaceTermHandler getStopWordHandler();

    public void setStopWordHandler(InterfaceTermHandler stopWordHandler);

    public ArrayList<String> tokenizeToArrayList(String input);

    public Set<String> tokenizeToSet(String input);
}

