package metrics;

import metrics.tokenisers.InterfaceTokeniser;
import metrics.tokenisers.TokeniserQGram3Extended;

import java.io.Serializable;
import java.util.*;

public final class QGramsDistance extends AbstractStringMetric implements Serializable {

	private static final long serialVersionUID = 991983085343042434L;

    private static final float EST_TIM_CONST = 1.34e-4f;

    private InterfaceTokeniser tokeniser;

    public QGramsDistance() {
    	super();
        tokeniser = new TokeniserQGram3Extended();
    }

    public QGramsDistance(final InterfaceTokeniser tokeniserToUse) {
    	super();
        tokeniser = tokeniserToUse;
    }

    public String getShortDescriptionString() {
        return "QGramsDistance";
    }

    public String getLongDescriptionString() {
        return "Implements the Q Grams Distance algorithm providing a similarity measure between two strings using the qGram approach check matching qGrams/possible matching qGrams";
    }

    public String getSimilarityExplained(final String string1, final String string2) {
        //todo this should explain the operation of a given comparison
        return null;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public float getSimilarityTimingEstimated(final String string1, final String string2) {
        //timed millisecond times with string lengths from 1 + 50 each increment
        //0.01	0.51	1.48	3.08	5.51	7.81	11.94	15.62	20.3	25.38	31.29	36.5	43.8	50.75	62.5	67.67	78	88.67	93.67	117	125	133	148.5	172	171.5	204	218.5	219	219	296	282	281	344	312	360	375	390	422	438	453	484	500	516	562	563	594	625	656	656	703	719	766	765	813	828	875	891	937	953	985
        final float str1Length = string1.length();
        final float str2Length = string2.length();
        return (str1Length * str2Length) * EST_TIM_CONST;
    }

    public float getSimilarity(final String string1, final String string2) {
    	
        final ArrayList<String> str1Tokens = tokeniser.tokenizeToArrayList(string1);
        final ArrayList<String> str2Tokens = tokeniser.tokenizeToArrayList(string2);

        final int maxQGramsMatching = str1Tokens.size() + str2Tokens.size();
        
        float ret;

        //return
        if (maxQGramsMatching == 0) {
            ret = 0.0f;
        } else {
            ret = (maxQGramsMatching - getUnNormalisedSimilarity(string1, string2)) / (float) maxQGramsMatching;
        }
        
        return ret;
        
    }

    public float getUnNormalisedSimilarity(final String string1, final String string2) {
        final ArrayList<String> str1Tokens = tokeniser.tokenizeToArrayList(string1);
        final ArrayList<String> str2Tokens = tokeniser.tokenizeToArrayList(string2);

        final Set<String> allTokens = new HashSet<String>();
        allTokens.addAll(str1Tokens);
        allTokens.addAll(str2Tokens);

        final Iterator<String> allTokensIt = allTokens.iterator();
        int difference = 0;
        while (allTokensIt.hasNext()) {
            final String token = allTokensIt.next();
            int matchingQGrams1 = 0;
            for (String str1Token : str1Tokens) {
                if (str1Token.equals(token)) {
                    matchingQGrams1++;
                }
            }
            int matchingQGrams2 = 0;
            for (String str2Token : str2Tokens) {
                if (str2Token.equals(token)) {
                    matchingQGrams2++;
                }
            }
            if (matchingQGrams1 > matchingQGrams2) {
                difference += (matchingQGrams1 - matchingQGrams2);
            } else {
                difference += (matchingQGrams2 - matchingQGrams1);
            }
        }

        //return
        return difference;
    }

	public InterfaceTokeniser getTokeniser() {
		return tokeniser;
	}

	public void setTokeniser(final InterfaceTokeniser tokeniser) {
		this.tokeniser = tokeniser;
	}
	
}


