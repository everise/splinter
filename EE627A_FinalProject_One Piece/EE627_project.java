
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;


public class EE627_project  {
	static RelatData[] track= new RelatData[296111];
	static Sixtrack[] tempRate= new Sixtrack[15716];
	static Sixtrack[] Results=new Sixtrack[15716];
	static UserData[] Des= new UserData[15716];
	static UserData[] Rates= new UserData[40265];
   
    
	public void read_track() throws IOException {
		UserData tempUser = new UserData();
		String temp;
		String[] tempstr;
		FileReader fr=new FileReader("src/testIdx2.txt");
		BufferedReader br=new BufferedReader(fr);
		int i = 0,len = 0;
		while((temp=br.readLine())!=null){
				
			if (temp.contains("|")){
				Des[i++]=tempUser;
				tempUser= new UserData();
				tempstr= temp.split("\\|");
				tempUser.user=Integer.parseInt(tempstr[0]);
				tempUser.track=new int[6];
				len=0;	
			}
			else{
				tempUser.track[len++]=Integer.parseInt(temp);
			}			
	
        }
		Des[i]=tempUser;
		
		br.close();	
		
	}
	public void read_rate() throws IOException {

		UserData tempUser = new UserData();
			
		String temp;
		String[] tempstr;
		FileReader fr=new FileReader("src/trainIdx2.txt");
		BufferedReader br=new BufferedReader(fr);
		int count = 0;
		while((temp=br.readLine())!=null){				
			if (temp.contains("|")){
				Rates[tempUser.user]=tempUser;
				tempUser= new UserData();
				tempstr= temp.split("\\|");
				//i=i+1;
				tempUser.user=Integer.parseInt(tempstr[0]);
				tempUser.len=Integer.parseInt(tempstr[1]);
				tempUser.track=new int[tempUser.len*2];
				count=0;			
			}
			else{
				String[] rates=temp.split("	");
				tempUser.track[count]=Integer.parseInt(rates[0]);
				tempUser.track[count+tempUser.len]=Integer.parseInt(rates[1]);
				count++;
			}			
	
        }
		Rates[tempUser.user]=tempUser;
		br.close();
	}
	public void read_track_data() throws IOException {
		String temp;
		String[] tempstr;
		FileReader fr=new FileReader("src/trackData2.txt");
		BufferedReader br=new BufferedReader(fr);
		while((temp=br.readLine())!=null){
				
			RelatData temptrack= new RelatData();
			tempstr= temp.split("\\|");
			temptrack.len=tempstr.length;
			temptrack.track=Integer.parseInt(tempstr[0]);
			if(!tempstr[1].equals("None")){
				temptrack.album=Integer.parseInt(tempstr[1]);
			}
			if(!tempstr[2].equals("None")){
				temptrack.artist=Integer.parseInt(tempstr[2]);
			}
			if(tempstr.length>3){
				temptrack.genre=new int[tempstr.length-3];
				for (int i = 0; i < tempstr.length-3; i++) {
					temptrack.genre[i]=Integer.parseInt(tempstr[i+3]);
				}
			}
			track[temptrack.track]=temptrack;
		}
		br.close();
	
	}
	
	
	public void calculate(){
		//algorithm to caculate the score 0.2075
				for(int i=1;i<15716;i++){
					Sixtrack temp= new Sixtrack();
					for(int j=0;j<6;j++){
						double tempscore_track=0;
						double tempscore_album=0;
						double tempscore_artist=0;
						double tempscore_genre=0;
						int count=0;
						int count_track=0;
						int count_album=0;
						int count_artist=0;
						int count_genre=0;
						for (int n=0;n<(Rates[Des[i].user].len);n++){
							if (  Des[i].track[j]==Rates[Des[i].user].track[n]  ){
								tempscore_track+= Rates[Des[i].user].track[n+Rates[Des[i].user].len];
								count_track++;
							}
							else if (  track[Des[i].track[j]].album==Rates[Des[i].user].track[n]  ){
								tempscore_album+= Rates[Des[i].user].track[n+Rates[Des[i].user].len];
								count_album++;
							}
							else if (  track[Des[i].track[j]].artist==Rates[Des[i].user].track[n]  ){
								tempscore_artist+= Rates[Des[i].user].track[n+Rates[Des[i].user].len];
								count_artist++;
							}
							else for (int m=0;m< (track[Des[i].track[j]].len-3 );m++){
								if (  track[Des[i].track[j]].genre[m]==Rates[Des[i].user].track[n]  ){
									tempscore_genre+= Rates[Des[i].user].track[n+Rates[Des[i].user].len];
									count_genre++;
								}
							}
						}
						count=(count_track+count_album+count_artist+count_genre);
						if (count!=0){
							if (count_genre!=0)
								temp.score[j]=tempscore_track*0.436+tempscore_album*0.912+tempscore_artist*0.591+tempscore_genre*0.05;
							else 
								temp.score[j]=tempscore_track*0.436+tempscore_album*0.912+tempscore_artist*0.651;
						}
						else{
						temp.score[j]=0;
						}				
					}
					tempRate[i]=temp;
				}
			}
	
	public void data_order(){
		for (int i=1;i<15716;i++){
		    Sixtrack binarySample= new Sixtrack();
		    int[] index=new int [3];		
			double temp=0;
			for(int l=0;l<6;l++){
				if (tempRate[i].score[l]>=temp){
					temp=tempRate[i].score[l];
					index[0]=l;
				}
			}
			temp=0;
			for(int l=0;l<6;l++){
				if (tempRate[i].score[l]>=temp&&l!=index[0]){
					temp=tempRate[i].score[l];
					index[1]=l;
				}
			}
			temp=0;
			for(int l=0;l<6;l++){
				if (tempRate[i].score[l]>=temp&&l!=index[0]&&l!=index[1]){
					temp=tempRate[i].score[l];
					index[2]=l;
				}
			}
			for(int j=0;j<3;j++){
				binarySample.score[index[j]]=1;
			}
			Results[i]=binarySample;
			}
		}
	public static void main(String[] args) throws IOException {

        EE627_project hw= new EE627_project();
        hw.read_track();
        hw.read_rate();
        hw.read_track_data();
        hw.calculate();
        hw.data_order();
        int m;
		try(FileWriter fw=new FileWriter("src\\result.txt")) {
			for (int i = 1; i < 15716; i++) {
				for (int j = 0; j < 6; j++) {
					m=(int) Results[i].score[j];
					fw.append(m+"\n");
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
  
