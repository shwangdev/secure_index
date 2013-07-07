package metrics;

import metrics.costfunctions.AbstractSubstitutionCost;
import metrics.math.MathFuncs;
import metrics.costfunctions.SubCost1_Minus2;

import java.io.Serializable;

public final class SmithWaterman extends AbstractStringMetric implements Serializable {

	private static final long serialVersionUID = -439961078368255776L;

    private static final float EST_TIM_CONST = 1.61e-4f;

    private AbstractSubstitutionCost dCostFunc;

    private float gapCost;


    public SmithWaterman() {
    	super();
        //set the gapCost to a default value
        gapCost = 0.5f;
        //set the default cost func
        dCostFunc = new SubCost1_Minus2();
    }

    public SmithWaterman(final float costG) {
    	super();
        //set the gapCost to a given value
        gapCost = costG;
        //set the cost func to a default function
        dCostFunc = new SubCost1_Minus2();
    }

    public SmithWaterman(final float costG, final AbstractSubstitutionCost costFunc) {
    	super();
        //set the gapCost to the given value
        gapCost = costG;
        //set the cost func
        dCostFunc = costFunc;
    }

    public SmithWaterman(final AbstractSubstitutionCost costFunc) {
    	super();
        //set the gapCost to a default value
        gapCost = 0.5f;
        //set the cost func
        dCostFunc = costFunc;
    }

    public float getGapCost() {
        return gapCost;
    }

    public void setGapCost(final float gapCost) {
        this.gapCost = gapCost;
    }

    public AbstractSubstitutionCost getdCostFunc() {
        return dCostFunc;
    }

    public void setdCostFunc(final AbstractSubstitutionCost dCostFunc) {
        this.dCostFunc = dCostFunc;
    }

    public String getShortDescriptionString() {
        return "SmithWaterman";
    }

    public String getLongDescriptionString() {
        return "Implements the Smith-Waterman algorithm providing a similarity measure between two string";
    }

    public String getSimilarityExplained(final String string1, final String string2) {
        //todo this should explain the operation of a given comparison
        return null;  //To change body of implemented methods use File | Settings | File Templates.
    }

    public float getSimilarityTimingEstimated(final String string1, final String string2) {
        //timed millisecond times with string lengths from 1 + 50 each increment
        //0	0.58	1.49	3.17	5.97	8.83	12.69	18.45	22.56	31.29	36.5	40.6	58.5	62.5	78.33	78	93.67	101.5	125	125	164	172	187.5	203	235	265	235	328	328	344	343	391	391	453	453	484	547	578	594	625	641	672	718	766	797	812	860	890	907	953	984	1078	1047	1094	1156	1188	1250	1328	1344	1390
        final float str1Length = string1.length();
        final float str2Length = string2.length();
        return ((str1Length * str2Length) + str1Length + str2Length) * EST_TIM_CONST;
    }

    public float getSimilarity(final String string1, final String string2) {
    	
        final float smithWaterman = getUnNormalisedSimilarity(string1, string2);

        //normalise into zero to one region from min max possible
        float maxValue = Math.min(string1.length(), string2.length());
        float ret;
        
        if (dCostFunc.getMaxCost() > -gapCost) {
            maxValue *= dCostFunc.getMaxCost();
        } else {
            maxValue *= -gapCost;
        }

        //check for 0 maxLen
        if (maxValue == 0) {
            ret = 1.0f; //as both strings identically zero length
        } else {
            //return actual / possible NeedlemanWunch distance to get 0-1 range
            ret = (smithWaterman / maxValue);
        }
        return ret;
    }

    public float getUnNormalisedSimilarity(final String s, final String t) {
        float[][] d; // matrix
        int n; // length of s
        int m; // length of t
        int i; // iterates through s
        int j; // iterates through t
        float cost; // cost

        // check for zero length input
        n = s.length();
        m = t.length();
        if (n == 0) {
            return m;
        }
        if (m == 0) {
            return n;
        }

        //create matrix (n)x(m)
        d = new float[n][m];

        //process first row and column first as no need to consider previous rows/columns
        float maxSoFar = 0.0f;
        
        for (i = 0; i < n; i++) {
            // get the substution cost
            cost = dCostFunc.getCost(s, i, t, 0);

            if (i == 0) {
                d[0][0] = MathFuncs.max3(0,
                        -gapCost,
                        cost);
            } else {
                d[i][0] = MathFuncs.max3(0,
                        d[i - 1][0] - gapCost,
                        cost);
            }
            //update max possible if available
            if (d[i][0] > maxSoFar) {
                maxSoFar = d[i][0];
            }
        }
        
        for (j = 0; j < m; j++) {
            // get the substution cost
            cost = dCostFunc.getCost(s, 0, t, j);

            if (j == 0) {
                d[0][0] = MathFuncs.max3(0,
                        -gapCost,
                        cost);
            } else {
                d[0][j] = MathFuncs.max3(0,
                        d[0][j - 1] - gapCost,
                        cost);
            }
            //update max possible if available
            if (d[0][j] > maxSoFar) {
                maxSoFar = d[0][j];
            }
        }

        // cycle through rest of table filling values from the lowest cost value of the three part cost function
        for (i = 1; i < n; i++) {
            for (j = 1; j < m; j++) {
                // get the substution cost
                cost = dCostFunc.getCost(s, i, t, j);

                // find lowest cost at point from three possible
                d[i][j] = MathFuncs.max4(0,
                        d[i - 1][j] - gapCost,
                        d[i][j - 1] - gapCost,
                        d[i - 1][j - 1] + cost);
                //update max possible if available
                if (d[i][j] > maxSoFar) {
                    maxSoFar = d[i][j];
                }
            }
        }

        // return max value within matrix as holds the maximum edit score
        return maxSoFar;
        
    }
    
}

