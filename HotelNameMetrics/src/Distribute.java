import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

public class Distribute {

	// The stored location.
	private static final String result_path = "D:\\Distribute";

	// Path of sheet1
	private static final String sheet1_path = "Distribute Normalised Hotel IDs sheet1.txt";
	// Path of sheet2
	private static final String sheet2_path = "Distribute Normalised Hotel IDs sheet2.txt";

	// The countryMap<Country_LocationID,corresponding directory>
	private Map<String, tempEntity> countryMap = new HashMap<String, tempEntity>();
	// The regionMap<Region_LocationID,corresponding directory>
	private Map<String, tempEntity> regionMap = new HashMap<String, tempEntity>();
	// The resortMap<Resort_LocationID,corresponding directory>
	private Map<String, tempEntity> resortMap = new HashMap<String, tempEntity>();

	/**
	 * main method
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		long a = System.currentTimeMillis();
		System.out.println("wait a minute...\r\n");
		new Distribute();
		System.out.println("end...");
		long b = System.currentTimeMillis();
		System.out.println();
		System.out.println("Total time is " + (b - a) + "ms");
	}

	/**
	 * constructor
	 */
	public Distribute() {
		try {
			// The method to read sheet2,generate all directory of
			// country/region/resort and store the id, path to their MAP.
			readSheet2();
			// The method to read sheet1,and store data into TXT files according
			// to their LocationID and Type.
			readSheet1();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 1.Read Sheet1���Based on LocationID,Type, 
	 * 2.Obtain the corresponding directory path from their respective maps 
	 * 3.Store the data into TXT format under their specific directory
	 * 
	 * @throws Exception
	 */
	public void readSheet1() throws Exception {
		File file = new File(sheet1_path);
		if (file.exists()) {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String str = null;
			int i = 0;
			while ((str = reader.readLine()) != null) {
				i++;
				if (!str.trim().equals("") && i > 1) {
					String[] strs = str.split(",");
					if (strs.length > 4) {
						File gfile = null;
						if (toTrim(strs[3]).equals("DestinationCountry")
								&& countryMap.containsKey(toTrim(strs[2]))
								&& countryMap.get(toTrim(strs[2]))
										.getLocataionName()
										.equals(convert(strs[4]))) {
							gfile = new File(countryMap.get(toTrim(strs[2]))
									.getPath()
									+ "\\"
									+ toTrim(strs[2])
									+ ".txt");
						} else if (toTrim(strs[3]).equals("DestinationRegion")
								&& regionMap.containsKey(strs[2])
								&& regionMap.get(toTrim(strs[2]))
										.getLocataionName()
										.equals(convert(strs[4]))) {
							gfile = new File(regionMap.get(toTrim(strs[2]))
									.getPath()
									+ "\\"
									+ toTrim(strs[2])
									+ ".txt");
						} else if (toTrim(strs[3]).equals("DestinationResort")
								&& resortMap.containsKey(strs[2])
								&& resortMap.get(toTrim(strs[2]))
										.getLocataionName()
										.equals(convert(strs[4]))) {
							gfile = new File(resortMap.get(toTrim(strs[2]))
									.getPath()
									+ "\\"
									+ toTrim(strs[2])
									+ ".txt");
						}
						if (gfile != null) {
							if (!gfile.exists()) {
								FileWriter fw = new FileWriter(gfile, true);
								PrintWriter pw = new PrintWriter(fw);
								pw.write("HotelId,AccommodationName,LocationID,LocationType,LocationName\r\n");
								pw.close();
							}
							FileWriter fw = new FileWriter(gfile, true);
							PrintWriter pw = new PrintWriter(fw);
							pw.write(strs[0] + "," + strs[1] + "," + strs[2]
									+ "," + strs[3] + "," + strs[4] + "\r\n");
							pw.close();
						}
					}
				}
			}
			reader.close();
		}
	}

	/**
	 * 1.Read sheet2 
	 * 2.Generate directories named as locationName under result_path,
	 * 		following the path format country/region/resort
	 * 3.Store the ID and path of country,region,resort into their MAP, respectively.
	 * 
	 * @throws Exception
	 */
	public void readSheet2() throws Exception {
		File file = new File(sheet2_path);
		if (file.exists()) {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String str = null;
			int i = 0;
			while ((str = reader.readLine()) != null) {
				i++;
				if (!str.trim().equals("") && i > 1) {
					String[] strs = str.split(",");
					File gfile = null;
					if (strs.length == 2) {
						gfile = new File(result_path + "\\" + convert(strs[1]));
						insertMap(countryMap, toTrim(strs[0]),
								convert(strs[1]), result_path + "\\"
										+ convert(strs[1]));
					} else if (strs.length == 4) {
						gfile = new File(result_path + "\\" + convert(strs[1])
								+ "\\" + convert(strs[3]));
						insertMap(countryMap, toTrim(strs[0]),
								convert(strs[1]), result_path + "\\"
										+ convert(strs[1]));
						insertMap(regionMap, toTrim(strs[2]), convert(strs[3]),
								result_path + "\\" + convert(strs[1]) + "\\"
										+ convert(strs[3]));
					} else if (strs.length == 6) {
						gfile = new File(result_path + "\\" + convert(strs[1])
								+ "\\" + convert(strs[3]) + "\\"
								+ convert(strs[5]));
						insertMap(countryMap, toTrim(strs[0]),
								convert(strs[1]), result_path + "\\"
										+ convert(strs[1]));
						insertMap(regionMap, toTrim(strs[2]), convert(strs[3]),
								result_path + "\\" + convert(strs[1]) + "\\"
										+ convert(strs[3]));
						insertMap(resortMap, toTrim(strs[4]), convert(strs[5]),
								result_path + "\\" + convert(strs[1]) + "\\"
										+ convert(strs[3]) + "\\"
										+ convert(strs[5]));
					}
					if (gfile != null && !gfile.exists()) {
						gfile.mkdirs();
					}
				}
			}
			reader.close();
		}
	}

	/**
	 * Convert some particular symbols involved in folder names.
	 * 
	 * @param folder
	 * @return
	 */
	public String convert(String folder) {
		return folder.trim().replaceAll("/", " or ");
	}

	/**
	 * Remove the leading and trailing whitespace of directory
	 * 
	 * @return
	 */
	public String toTrim(String folder) {
		return folder.trim();
	}

	/**
	 *  Store the locationId, directory path into corresponding MAP
	 * 
	 * @param map
	 * @param locationId
	 * @param path
	 */
	public void insertMap(Map<String, tempEntity> map, String locationId,
			String locationName, String path) {
		if (!map.containsKey(locationId)) {
			map.put(locationId, new tempEntity(path, locationName));
		}
	}

	class tempEntity {
		private String path;

		private String locataionName;

		public tempEntity() {
			super();
		}

		public tempEntity(String path, String locataionName) {
			super();
			this.path = path;
			this.locataionName = locataionName;
		}

		public String getPath() {
			return path;
		}

		public void setPath(String path) {
			this.path = path;
		}

		public String getLocataionName() {
			return locataionName;
		}

		public void setLocataionName(String locataionName) {
			this.locataionName = locataionName;
		}

	}
}
