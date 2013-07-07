package metrics;

import java.util.Arrays;

public abstract class AbstractStringMetric implements InterfaceStringMetric {

    public abstract String getShortDescriptionString();

    public abstract String getLongDescriptionString();

    public abstract String getSimilarityExplained(String string1, String string2);
    
    private transient float[] results;

    public final long getSimilarityTimingActual(final String string1, final String string2) {
        //initialise timing
        final long timeBefore = System.currentTimeMillis();
        //perform measure
        getSimilarity(string1, string2);
        //get time after process
        final long timeAfter = System.currentTimeMillis();
        //output time taken
        return timeAfter - timeBefore;
    }

    public final float[] batchCompareSet(final String[] set, final String comparator) {
        this.results = new float[set.length];
        for(int strNum=0; strNum<set.length; strNum++) {
            //perform similarity test
            results[strNum] = getSimilarity(set[strNum],comparator);
        }
        return Arrays.copyOf(this.results, this.results.length);
    }

    public final float[] batchCompareSets(final String[] firstSet, final String[] secondSet) {
        //sets the results to equal the shortest string length should they differ.
        if(firstSet.length <= secondSet.length) {
            this.results = new float[firstSet.length];
        } else {
        	this.results = new float[secondSet.length];
        }
        for(int strNum=0; strNum<results.length; strNum++) {
            //perform similarity test
        	this.results[strNum] = getSimilarity(firstSet[strNum],secondSet[strNum]);
        }
        return Arrays.copyOf(this.results, this.results.length);
    }

    public abstract float getSimilarityTimingEstimated(final String string1, final String string2);


    public abstract float getSimilarity(String string1, String string2);

    public abstract float getUnNormalisedSimilarity(String string1, String string2);
}
