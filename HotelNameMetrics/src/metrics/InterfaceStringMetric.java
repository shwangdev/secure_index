package metrics;

public interface InterfaceStringMetric {

    String getShortDescriptionString();

    String getLongDescriptionString();

    long getSimilarityTimingActual(String string1, String string2);

    float getSimilarityTimingEstimated(String string1, String string2);

    float getSimilarity(String string1, String string2);

    String getSimilarityExplained(String string1, String string2);
}
