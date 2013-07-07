package metrics;

import java.util.ArrayList;
import java.util.List;

public class StopWord {

	private static List<String> wordList;
	static {
		wordList = new ArrayList<String>();
		wordList.add("Hotel");
	}
	
	public static void addWord(String word) {
		wordList.add(word);
	}
	
	public static String remove(String source) {
		StringBuilder result = new StringBuilder();
		String[] words = source.split(" ");
		for(String word : words) {
			if(!wordList.contains(word)) {
				result.append(word);
				result.append(" ");
			}
		}
		
		return result.toString().trim();
	}
}
