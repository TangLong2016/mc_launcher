import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;


public class CraftBoxHelper {
	public native void onCreateHelper();
	public native void onJoinGame();
    public native void onDisconnectGame();

    protected bao mcInst;

	private static String version = "1.0";

	public void joinGame(){
		try{
			onJoinGame();
		}catch(Throwable e){
			e.printStackTrace();
		}

	}

	public void disconnectGame(){
		try{
			onDisconnectGame();
		}catch(Throwable e){
			e.printStackTrace();
		}
	}
    
    public boolean isSinglePlay(){
    	return mcInst.G();
    }
    
    public void sendChatMessage(String msg){
    	logger.info(msg);
    	if(mcInst.h != null)
    		mcInst.h.a(msg);
    }

	public static Logger logger = LogManager.getLogger(ModInformation.NAME);
    
    static{
    	try{
    		if(System.getProperty("os.arch").compareToIgnoreCase("x86") == 0)
    			System.loadLibrary("CraftBoxHelper32");
    		else
    			System.loadLibrary("CraftBoxHelper64");
    	}catch(Throwable exception){
    		logger.error("can not load CraftBoxHelper");
    	}
    }
    
    public CraftBoxHelper(bao m){
    	mcInst = m;
    	
    	try{
    		onCreateHelper();
    	}catch(Throwable a){
    		a.printStackTrace();
    	}
    	
    }

	public static void main(String[] args) {
		System.out.print("CraftBoxHelper "+CraftBoxHelper.version);
	}
}
