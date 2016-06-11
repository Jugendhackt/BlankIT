import java.util.Date;
import java.util.Formatter;
import java.util.Locale;

import java.lang.Thread;

import java.text.SimpleDateFormat;

/**
 * Debug enum containing the different debug levels used by Debug class
 */
enum DebugLevel
{
	MINIMAL, LOW, MEDIUM, HIGH;
}


/**
 * Main Debug class containing a collection of debug methods.
 */
public class Debug
{
    private static void out(String prefix, String text, DebugLevel debugLevel)
    {
    	StringBuilder sb = new StringBuilder();
    	Formatter formatter = new Formatter(sb, Locale.US);
    	
        switch (debugLevel)
        {
        	case LOW:
        		formatter.format("[%s@%s]: %s", prefix, GetCallerClassName(), text);
        		break;
        	case MEDIUM:
        		formatter.format("[%s@%s@%s]: %s", prefix, GetCallerMethodeName(),
        												   GetCallerClassName(), text);
        		break;
        	case HIGH:
        		formatter.format("[%s@%s@%s@%s.%-4d]: %s", prefix, GetCallerMethodeName(),
															  	   GetCallerClassName()  , 
															  	   GetCallerFileName()   , 
															  	   GetCallerLineNumber() ,
															  	   text);
        		break;
        	default:
        		formatter.format("[%s]: %s", prefix, text);
        		break;
        }
        
        System.out.println(sb.toString());
        formatter.close();
    }
    

	/**
	 * Splits sentences in word lists.
	 *
	 * @param  sentence  One sentence represented as a String.
	 * @return	Returns a String array containing the individual words if the sentence.
	 */
    public static void Clear()
    {
        System.out.print("\f");
    }
    
    public static void Seperator()
    {
    	System.out.println();
    }
    public static void Log(String text) 
    { 
        out("DEBUG"		, text, DebugLevel.MINIMAL); 
    }    
    public static void Warning(String text)  
    { 
        out("WARNING"	, text, DebugLevel.LOW); 
    } 
    public static void Error(String text) 
    { 
        out("ERROR"		, text, DebugLevel.HIGH); 
        System.exit(1);
    } 
    public static void Todo()
    { 
        out("TODO"		, "TODO CALLED IN CODE", DebugLevel.HIGH); 
    } 

    private static String GetCallerMethodeName(){ return GetStackTraceElement().getMethodName() ; }
    private static String GetCallerClassName() 	{ return GetStackTraceElement().getClassName() 	; }
    private static String GetCallerFileName() 	{ return GetStackTraceElement().getFileName() 	; }
    private static int GetCallerLineNumber() 	{ return GetStackTraceElement().getLineNumber()	; }
    
    private static StackTraceElement GetStackTraceElement()
    {
        StackTraceElement[] stElements = Thread.currentThread().getStackTrace();
        for (int i=1; i<stElements.length; i++) {
            StackTraceElement ste = stElements[i];
            if (!ste.getClassName().equals(Debug.class.getName())  &&
                ste.getClassName().indexOf("java.lang.Thread")!=0) {
                return ste;
            }
        }
        return null;
    }
    public static String GetCurrentTimeStamp() 
    {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss.SSS");//"yyyy-MM-dd HH:mm:ss.SSS"
        return sdf.format(new Date()); //now = new Date();
    }
}
