import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Utility{
	static Map<String,List<String>> map = new HashMap<String,List<String>>();
    static ArrayList<String> charList = new ArrayList<String>();//非终结符
    static ArrayList<String> terminalList = new ArrayList<>();//终结符
    static Map<String, ArrayList<String>> m = new HashMap<>();
    static Map<String, ArrayList<String>> follow = new HashMap<>();
    
    //
    static ArrayList<String> firstSet(String pr){
    	ArrayList<String> tempFirst = new ArrayList<>();
	    if(charList.contains(pr.charAt(0)+"")){
	        int i,j;
	        i=0;
	        int flag=1;
	        while(i<pr.length() && charList.contains(pr.charAt(i)+"") && flag==1){
	            char current = pr.charAt(i);
	            ArrayList<String> first = m.get(current+"");
	            flag=0;
	            for(j=0;j<first.size();j++){
	                if(!first.get(j).equals("e"))
	                    tempFirst.add(first.get(j));
	                else
	                    flag=1;
	            }
	            i++;
	        }
	        if(i == pr.length() && flag==1)
	            tempFirst.add("e");
	        else if(flag==1){
	            tempFirst.add(pr.charAt(i)+"");
	        }
	    }else{
	        tempFirst.add(pr.charAt(0)+"");
	    }
	    return  tempFirst;
    }

    static ArrayList<String> followSet(String variable){
        return follow.get(variable);
    }
    
    //非空终结符
    static void initialiseTerminals(){
        int i,j,k;
        for(i=0;i<charList.size();i++){
            String currentAi = charList.get(i);
            List<String> productionAi = map.get(currentAi);
            for(j=0;j<productionAi.size();j++){
                String currentClause =productionAi.get(j);
                for(k=0;k<currentClause.length();k++){
                    if(!charList.contains(currentClause.charAt(k)+"")&&!terminalList.contains(currentClause.charAt(k)+"")){
                        if(!(currentClause.charAt(k)+"").equals("e"))
                        terminalList.add(currentClause.charAt(k)+"");//非空终结符
                    }
                }
            }
        }
    }
}