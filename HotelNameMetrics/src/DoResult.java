import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import metrics.*;

public class DoResult {

	// The path of source folder
	private static final String source_path = "D:\\Distribute";

	private static final String result_path = "D:\\Distribute1";

	private String resultTxtPath;
	private int amount = 0;
	
	private static String similarityType;
	private static float threshold;
	private static StringMetricFacade metricFacade;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		metricFacade = new StringMetricFacade();
		
		if(args.length < 2 || !metricFacade.validMetricsType(args[0])) {
			printHelp();
			return;
		}
		
		try {
			threshold = Float.parseFloat(args[1]);
			if(threshold >=1 || threshold <= 0) {
				printHelp();
				return;
			}
		} catch(Exception ex) {
			printHelp();
			return;
		}
		
		if(args.length >=4 && args[2].equals("-stopword")) {
			for(String userStopWord : args[3].split(",")) {
				StopWord.addWord(userStopWord);
			}
		}
		
		similarityType = args[0];
		metricFacade.setMetrics(similarityType);
		metricFacade.setThreshold(threshold);
		
		
		long a = System.currentTimeMillis();
		System.out.println("wait a minute...\r\n");
		DoResult s = new DoResult();
		System.out.println("end...");
		long b = System.currentTimeMillis();
		System.out.println();
		System.out.println("Total number of lines in result is " + s.amount
				+ " and the total time is " + (b - a) + "ms");
	}
	
	private static void printHelp() {
		System.out.println("Hits: java DoResult [Whirl | Qgram | SmithWaterman] [Threshold(0<threshold<1)] [-stopword word1,word2,word3...]");
		System.out.println("Example: java DoResult Whril 0.5 -stopword hotel,is,am,at");
	}

	/**
	 * Constructor
	 */
	public DoResult() {
		createResultTxt();
		readFiles();
	}

	/**
	 * Read txt files from each level directories, and store the data to
	 * corresponding countryMap, regionMap, resortMap
	 * 
	 */
	public void readFiles() {
		try {
			MyFilenameFilter myFilenameFilter = new MyFilenameFilter();
			File topDirectory = new File(source_path);

			if (topDirectory.exists() && topDirectory.isDirectory()) {
				String[] countryDirectories = topDirectory.list();
				// The operation on country level directory <the first level
				// directory>
				for (String firstPath : countryDirectories) {

					String countryPath = topDirectory.getAbsolutePath() + "\\"
							+ firstPath;
					File countryDirectory = new File(countryPath);

					if (countryDirectory.isDirectory()) {
						Map<String, String> countryMap = new HashMap<String, String>();
						File[] countryFiles = countryDirectory
								.listFiles(myFilenameFilter);
						for (File file : countryFiles) {
							// Read txt file from country level directory
							this.readTxt(file.getAbsolutePath(), countryMap);
						}

						// The operation on region level directory <the second
						// level directory>
						String[] regionDirectories = countryDirectory.list();
						for (String secondPath : regionDirectories) {

							String regionPath = countryDirectory
									.getAbsolutePath() + "\\" + secondPath;
							File regionDirectory = new File(regionPath);

							if (regionDirectory.isDirectory()) {
								Map<String, String> regionMap = new HashMap<String, String>();
								File[] regionFiles = regionDirectory
										.listFiles(myFilenameFilter);
								for (File file : regionFiles) {
									// Read txt file from region level directory
									this.readTxt(file.getAbsolutePath(),
											regionMap);
								}

								// The operation on resort level directory <the
								// third level directory>
								String[] resortDirectories = regionDirectory
										.list();
								for (String thirdPath : resortDirectories) {
									String resortPath = regionDirectory
											.getAbsolutePath()
											+ "\\"
											+ thirdPath;
									File resortDirectory = new File(resortPath);

									if (resortDirectory.isDirectory()) {
										Map<String, String> resortMap = new HashMap<String, String>();
										File[] resortFiles = resortDirectory
												.listFiles(myFilenameFilter);
										for (File file : resortFiles) {
											// Read txt file from resort level
											// directory
											this.readTxt(
													file.getAbsolutePath(),
													resortMap);
										}
										processRealData(countryMap, regionMap,
												resortMap);
									}
								}
								processRealData(countryMap, regionMap);
							}
						}
						processRealData(countryMap);
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * countryMap operation: This is valid if current directory only has txt
	 * file.
	 * 
	 * @param countryMap
	 * @throws IOException
	 */
	public void processRealData(Map<String, String> countryMap)
			throws IOException {
		Map<String, String> deleteMap1 = new HashMap<String, String>();
		for (String key : countryMap.keySet()) {
			String name = countryMap.get(key);
			if (!deleteMap1.containsKey(key)) {
				StringBuffer data = new StringBuffer(key);
				// Calculate the similarity with the data from countryMap
				deleteMap1.put(key, name);
				this.calculateHotelName(data, countryMap, deleteMap1, name);
				// Write to result.txt
				this.writeToTxt(data.toString());
			}
		}
	}

	/**
	 * countryMap and regionMap operation: This is valid if current directory
	 * only has two layer, which means it may have both country level txt and
	 * region level txt.
	 * 
	 * @param countryMap
	 * @param regionMap
	 * @throws IOException
	 */
	public void processRealData(Map<String, String> countryMap,
			Map<String, String> regionMap) throws IOException {
		Map<String, String> deleteMap1 = new HashMap<String, String>();
		Map<String, String> deleteMap2 = new HashMap<String, String>();
		for (String key : regionMap.keySet()) {

			String name = regionMap.get(key);
			if (!deleteMap1.containsKey(key)) {
				StringBuffer data = new StringBuffer(key);
				// Calculate the similarity with the data from regionMap
				deleteMap1.put(key, name);
				this.calculateHotelName(data, regionMap, deleteMap1, name);
				// Calculate the similarity with the data from countryMap
				this.calculateHotelName(data, countryMap, deleteMap2, name);
				for (String key2 : deleteMap2.keySet()) {
					countryMap.remove(key2);
				}
				deleteMap2.clear();
				// Write to result.txt
				this.writeToTxt(data.toString());
			}
		}

	}

	/**
	 * countryMap, regionMap and resortMap operation: This is valid if current
	 * directory has three layers, which means it has country level, region
	 * level and resort level txt files.
	 * 
	 * @param countryMap
	 * @param regionMap
	 * @param resortMap
	 * @throws IOException
	 */
	public void processRealData(Map<String, String> countryMap,
			Map<String, String> regionMap, Map<String, String> resortMap)
			throws IOException {
		Map<String, String> deleteMap1 = new HashMap<String, String>();
		Map<String, String> deleteMap2 = new HashMap<String, String>();
		Map<String, String> deleteMap3 = new HashMap<String, String>();
		for (String key : resortMap.keySet()) {

			String name = resortMap.get(key);
			if (!deleteMap1.containsKey(key)) {
				StringBuffer data = new StringBuffer(key);
				// Calculate the similarity with the data from resortMap
				deleteMap1.put(key, name);
				this.calculateHotelName(data, resortMap, deleteMap1, name);
				// Calculate the similarity with the data from regionMap
				this.calculateHotelName(data, regionMap, deleteMap2, name);
				for (String key2 : deleteMap2.keySet()) {
					regionMap.remove(key2);
				}
				deleteMap2.clear();
				// Calculate the similarity with the data from countryMap
				this.calculateHotelName(data, countryMap, deleteMap3, name);
				for (String key2 : deleteMap3.keySet()) {
					countryMap.remove(key2);
				}
				deleteMap3.clear();
				// Write to result.txt
				this.writeToTxt(data.toString());
			}
		}
	}

	/**
	 * Calculate the similarity between the data from Map and current hotel
	 * name. If it returns high similarity score, then delete the data from Map.
	 * 
	 * @param data
	 * @param map
	 * @param name
	 */
	public void calculateHotelName(StringBuffer data, Map<String, String> map,
			Map<String, String> deleteMap, String name) {
		for (String key : map.keySet()) {
			String name2 = map.get(key);
			if (!deleteMap.containsKey(key)) {
				float similarity = calculate(name, name2);
				if(similarity != -1.0f) {
					data.append("\r\n");
					data.append(key);
					data.append(" [");
					data.append(similarity);
					data.append("]");
					deleteMap.put(key, name2);
				}
				
			}
		}
	}

	/**
	 * The collection for similarity algorithms
	 * 
	 * @param name1
	 * @param name2
	 * @return
	 */
	public float calculate(String name1, String name2) {
		float similarity =  metricFacade.getSimilarity(name1, name2);
		return metricFacade.isSimilar(similarity) ? similarity : -1.0f;
	}

	/**
	 * Read txt file£¬and store the HotelId, AccommodationName(stop word
	 * removed)into map
	 * 
	 * @param txtPath
	 * @param map
	 */
	public void readTxt(String txtPath, Map<String, String> map)
			throws Exception {
		File file = new File(txtPath);
		if (file.exists()) {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String str = null;
			int i = 0;
			while ((str = reader.readLine()) != null) {
				i++;
				if (!str.trim().equals("") && i > 1) {
					String[] strs = str.split(",");
					if (strs.length == 5) {
						map.put(strs[0], removeStopWord(strs[1]));
					}
				}
			}
		}
	}

	/**
	 * Remove stop word
	 * 
	 * @param word
	 * @return
	 */
	public String removeStopWord(String word) {
		return StopWord.remove(word);
	}

	/**
	 * Write the result data into result.txt
	 * 
	 * @param data
	 * @throws IOException
	 */
	public void writeToTxt(String data) throws IOException {
		amount++;
		FileWriter fw = new FileWriter(resultTxtPath, true);
		PrintWriter pw = new PrintWriter(fw);
		pw.write(amount + ": " + data + "\r\n\r\n");
		pw.close();
	}

	/**
	 * Create result.txt file
	 */
	public void createResultTxt() {
		try {
			File file = new File(result_path);
			if (!file.exists())
				file.mkdirs();
			resultTxtPath = result_path + "\\result-" + similarityType + "-" + threshold + ".txt";
			file = new File(resultTxtPath);
			int i = 0;
			while (file.exists()) {
				i++;
				resultTxtPath = result_path + "\\result-" + similarityType + "-" + threshold + "-" + i + ".txt";
				file = new File(resultTxtPath);
			}
			FileWriter fw = new FileWriter(file, true);
			PrintWriter pw = new PrintWriter(fw);
			pw.write("");
			pw.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * txt file filter
	 * 
	 */
	class MyFilenameFilter implements FilenameFilter {
		@Override
		public boolean accept(File file, String name) {
			File child = new File(file.getAbsolutePath() + "\\" + name);
			if (child.isFile() && name.toLowerCase().endsWith(".txt")) {
				return true;
			}
			return false;
		}

	}

}
