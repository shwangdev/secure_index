package metrics.wordhandlers;

public final class DummyStopTermHandler implements InterfaceTermHandler {

	private static final long serialVersionUID = 7599092558595710553L;

    public void addWord(final String termToAdd) {
        //empty call
    }

    public final String getShortDescriptionString() {
        return "DummyStopTermHandler";
    }

    public void removeWord(final String termToRemove) {
        //empty call
    }

    public int getNumberOfWords() {
        //always zero
        return 0;
    }

    public boolean isWord(final String termToTest) {
        return false;
    }

     public StringBuffer getWordsAsBuffer() {
        return new StringBuffer();
    }
}
