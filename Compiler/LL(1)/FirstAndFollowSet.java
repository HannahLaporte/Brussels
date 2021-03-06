import java.util.*;


public class FirstAndFollowSet {
    private Map<String,List<String>> map = new HashMap<String,List<String>>();
    private ArrayList<String> charList = new ArrayList<String>();
    private Map<String, ArrayList<String>> m = new HashMap<>();          // First Set
    private Map<String, ArrayList<String>> follow = new HashMap<>();     //  Follow Set
    
    public FirstAndFollowSet(Map<String,List<String>> m, ArrayList<String> a){
        map =m;
        charList = a;
    }

    public Map<String, List<String>> getMap(){
        return map;
    }

    public ArrayList<String> getCharList(){
        return charList;
    }

    public Map<String, ArrayList<String>> getFirst(){
        return m;
    }

    public Map<String, ArrayList<String>> getFollow(){
        return follow;
    }

    public void firstAndFollowSet(){
        int i, j, k;
        for(i=0;i<charList.size();i++){
            String ai=charList.get(i);
            List<String> productionAi=map.get(ai);
            ArrayList<String> tempFirst=new ArrayList<>();
            for(j=0;j<productionAi.size();j++) {//初步first,产生式头部中终结符
                if (!charList.contains(productionAi.get(j).charAt(0)+"")){
                    tempFirst.add(productionAi.get(j).charAt(0)+"");
                }
            }
            m.put(ai, tempFirst);//添加到First
        }
        
        //构造first集
        while (true){
            int flag=1;
            for(i=0;i<charList.size();i++){
                String ai=charList.get(i);
                List<String> productionAi=map.get(ai);
                for(j=0;j<productionAi.size();j++){
                    int con=0;
                    while(con>=0 && con < productionAi.get(j).length()) {
                        char start = productionAi.get(j).charAt(con);
                        int tempCon = con;
                        if (charList.contains(start+"") ){//非终结符
                            String tempString = start + "";
                            if (m.containsKey(tempString)) {
                                ArrayList<String> firstAj = m.get(tempString);
                                ArrayList<String> firstAi = m.get(ai);
                                for (k = 0; k < firstAj.size(); k++) {
                                    if (firstAj.contains("e")) {
                                        tempCon++;
                                        if (con == productionAi.get(j).length() - 1) {
                                            if (!firstAi.contains("e")) {
                                                firstAi.add("e");
                                                flag = 0;
                                            }
                                        }
                                    }
                                    if (!firstAi.contains(firstAj.get(k))) {
                                        if (!firstAj.get(k).equals("e")) {
                                            firstAi.add(firstAj.get(k));
                                            flag = 0;
                                        }
                                    }
                                }
                                m.replace(ai, firstAi);
                            }
                        } else {//终结符
                            ArrayList<String> firstAi = m.get(ai);
                            if (!firstAi.contains(start + "")) {
                                firstAi.add(start + "");
                                m.replace(ai, firstAi);
                                flag = 0;
                            }

                        }
                        if (tempCon > con) {
                            con++;
                        } else
                            con = -1;
                    }

                }
            }
            if (flag == 1)
                break;
        }

        Utility.m =m;
        ArrayList<String> temp = new ArrayList<>();
        temp.add("$");
        follow.put(charList.get(0), temp);//讲$符加入开始符follow集
        int itr = 1;
        itr = 0;
        for (i = 0; i < charList.size(); i++) {
	        String ai = charList.get(i);
	        List<String> productionAi = map.get(ai);
	        for (j = 0; j < productionAi.size(); j++) {
	            String current = productionAi.get(j);
	            for (k = 0; k < current.length() - 1; k++) {
	                String tempChar = current.charAt(k) + "";
	                if (charList.contains(tempChar)) {
	                    String pass = current.substring(k+1,current.length());
	                    ArrayList<String> res = Utility.firstSet(pass);
	                    if (follow.containsKey(tempChar)) {
	                            ArrayList<String> cat = follow.get(tempChar);
	                            for (int ii = 0; ii < res.size(); ii++) {
	                                if (!res.get(ii).equals("e") && !cat.contains(res.get(ii)))
	                                    cat.add(res.get(ii));
	                            }
	                            follow.replace(tempChar, cat);
	                        } else {
	                            ArrayList<String> cat = new ArrayList<>();
	                            for (int ii = 0; ii < res.size(); ii++) {
	                                if (!res.get(ii).equals("e") && !cat.contains(res.get(ii)))
	                                    cat.add(res.get(ii));
	                            }
	                            follow.put(tempChar, cat);
	                        }
	                    }
	                }
	            }
        }
        int flag;

        while (true){
            flag=1;
            for(i=0;i<charList.size();i++)
            {
                String ai = charList.get(i);
                List<String> productionAi = map.get(ai);
                for(j=0;j<productionAi.size();j++)
                {
                    String current = productionAi.get(j);
                    for(k=0;k<current.length()-1 ;k++)
                    {
                       if(charList.contains(current.charAt(k)+""))
                       {
                           String pass= current.substring(k+1,current.length());
                           ArrayList<String> res= Utility.firstSet(pass);
                           Scanner scanner = new Scanner(System.in);
                           if(res.contains("e"))
                           {
                               ArrayList<String> followB = follow.get(current.charAt(k)+"") ;
                               ArrayList<String> followA = follow.get(ai);
                               if(followA==null)
                                   followA = new ArrayList<>();
                               if(followB == null)
                                   followB = new ArrayList<>();
                               for(int ii =0;ii<followA.size();ii++)
                               {
                                   if(!followB.contains(followA.get(ii)))
                                   {
                                       followB.add(followA.get(ii));
                                       flag=0;
                                   }
                               }
                               follow.replace(current.charAt(k)+"",followB);
                           }
                       }
                    }
                    if(charList.contains(current.charAt(current.length()-1)+""))
                    {
                        //String aj = current.charAt(current.length()-1)+"";
                        ArrayList<String> followB =follow.get(current.charAt(current.length()-1)+"");
                        ArrayList<String> followA = follow.get(ai);
                        if(followA== null)
                            followA = new ArrayList<>();
                        if(followB == null)
                            followB  = new ArrayList<>();
                        for(int ii =0;ii<followA.size();ii++)
                        {
                            if(!followB.contains(followA.get(ii)) || followB.size()==0) {
                                followB.add(followA.get(ii));
                                flag = 0;
                            }

                        }
                        if (follow.containsKey(current.charAt(current.length()-1)+""))
                            follow.replace(current.charAt(current.length()-1)+"",followB);
                        else
                            follow.put(current.charAt(current.length()-1)+"",followB);

                    }
                }
            }
            if(flag==1)
            break;
        }

        Utility.follow = follow;
    }
}