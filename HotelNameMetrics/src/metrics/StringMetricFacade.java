package metrics;

public class StringMetricFacade {
	
	public final static String TYPE_WHIRL = "Whirl";
	public final static String TYPE_QGRAM = "Qgram";
	public final static String TYPE_SMITH = "SmithWaterman";
	
	private float threshold;
	private InterfaceStringMetric stringMetric;
	
	public Boolean validMetricsType(String type) {
		if(type.equalsIgnoreCase(TYPE_WHIRL) 
				|| type.equalsIgnoreCase(TYPE_QGRAM) 
				|| type.equalsIgnoreCase(TYPE_SMITH))
			return true;
		else 
			return false;
	}
	
	public void setThreshold(float threshold) {
		this.threshold = threshold;
	}
	
	public void setMetrics(String type) {
		if(type.equalsIgnoreCase(TYPE_WHIRL)) {
			stringMetric = new CosineSimilarity();
		} else if(type.equalsIgnoreCase(TYPE_QGRAM)) {
			stringMetric = new QGramsDistance();
		} else if(type.equalsIgnoreCase(TYPE_SMITH)) {
			stringMetric = new SmithWaterman();
		}
	}
	
	public float getSimilarity(String source, String dist) {
		return stringMetric.getSimilarity(source, dist);
	}
	
	public Boolean isSimilar(float similarity) {
		return similarity >= threshold;
	}
}
