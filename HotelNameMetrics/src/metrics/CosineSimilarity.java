package metrics;

import metrics.tokenisers.InterfaceTokeniser;
import metrics.tokenisers.TokeniserWhitespace;

import java.util.HashSet;
import java.util.Set;
import java.util.ArrayList;
import java.io.Serializable;

public final class CosineSimilarity extends AbstractStringMetric implements Serializable {

	private static final long serialVersionUID = -7811752242931658044L;

    private static final float EST_TIM_CONST = 0.00000038337142857142857142857142857142f;

    private InterfaceTokeniser tokeniser;

    public CosineSimilarity() {
    	super();
        tokeniser = new TokeniserWhitespace();
    }

    public CosineSimilarity(final InterfaceTokeniser tokeniserToUse) {
    	super();
        tokeniser = tokeniserToUse;
    }

    public String getShortDescriptionString() {
        return "CosineSimilarity";
    }

    public String getLongDescriptionString() {
        return "Implements the Cosine Similarity algorithm providing a similarity measure between two strings from the angular divergence within term based vector space";
    }

    public String getSimilarityExplained(final String string1, final String string2) {
        //todo this should explain the operation of a given comparison
        return null;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public float getSimilarityTimingEstimated(final String string1, final String string2) {
        //timed millisecond times with string lengths from 1 + 50 each increment
        //0	0.02	0.03	0.05	0.08	0.11	0.14	0.18	0.23	0.27	0.33	0.39	0.46	0.52	0.6	0.65	0.76	0.84	0.94	1.01	1.13	1.23	1.49	1.45	1.95	1.67	2.26	1.93	2.6	2.26	2.86	2.54	3.17	2.91	3.76	3.17	3.9	3.5	4.32	3.9	5.1	4.32	5.64	4.83	5.64	5.07	6.34	5.64	7.03	5.97	7.81	6.55	8.12	7	9.23	7.52	9.71	8.12	10.68	8.46
        final float str1Length = string1.length();
        final float str2Length = string2.length();
        return (str1Length + str2Length) * ((str1Length + str2Length) * EST_TIM_CONST);
    }

    public float getSimilarity(final String string1, final String string2) {
        final ArrayList<String> str1Tokens = tokeniser.tokenizeToArrayList(string1);
        final ArrayList<String> str2Tokens = tokeniser.tokenizeToArrayList(string2);

        final Set<String> allTokens = new HashSet<String>();
        allTokens.addAll(str1Tokens);
        final int termsInString1 = allTokens.size();
        final Set<String> secStrToks = new HashSet<String>();
        secStrToks.addAll(str2Tokens);
        final int termsInString2 = secStrToks.size();

        //now combine the sets
        allTokens.addAll(secStrToks);
        final int commonTerms = (termsInString1 + termsInString2) - allTokens.size();

        //return CosineSimilarity
        return (float) (commonTerms) / (float) (Math.pow((float) termsInString1, 0.5f) * Math.pow((float) termsInString2, 0.5f));
    }

    public float getUnNormalisedSimilarity(final String string1, final String string2) {
        return getSimilarity(string1, string2);
    }

	public InterfaceTokeniser getTokeniser() {
		return tokeniser;
	}

	public void setTokeniser(final InterfaceTokeniser tokeniser) {
		this.tokeniser = tokeniser;
	}
    
}



