Unreal – Android – Blueprint to Java

Purpose  

We may need to access to the mobile component like the camera or differents captors. Accessing to these components require an access to the Android Java Sdk.

Through this document, we will see how to communicate from Blueprint to C++ and from C++ to Java using JNI. We will also see how to use a jar file to keep all our class packaged. 

Blueprint -> C++ -> JNI -> Java -> Jar




Blueprint -> C++


In this part you will have a few informations, if you need more you can check the unreal tutorials
C++ Tutorial
Compilation
Blueprint Communication






We will work in JarTuto C++ Blank project. We will begin with a BlueprintCommunication Actor class and create a blueprint class from it.

To manage a communication from Blueprint to C++, we need to use a macro UPROPERTY() just above your function definition in h file.

UFUNCTION(BlueprintCallable, Category = "#Log")
void myLog();


Don't forget to implement you function in cpp file.

void ABlueprintCommunication::myLog() {
	// Your code
}



































- Call C++ Method from Blueprint -




You can also call a Blueprint event from your C++ code :

UFUNCTION(BlueprintImplementableEvent, Category = "#Log")
void CalledFromCpp(int value);



Use CalledFromCpp(200); in a C++ function to call your Blueprint event.
- You can't implement your CalledFromCpp(int value) function in cpp file. If you want to provide a default implemented function in case it's not used in Blueprint, take a look to BlueprintNativeEvent.


- You can't create a CalledFromCpp function in Blueprint. But you can create a #CalledFromCpp function which will be called from your event if you want.






























- Call Blueprint event from C++ -





























C++ -> JNI


For this part, we will create a new C++ Actor class called JavaCommunication 

JNI will only be available on your mobile, that's why we need to delimited our JNI code in cpp file by using

#if PLATFORM_ANDROID

#endif



We will need to include some tools to make it work. Place the code below inside the previous condition, on top of all methods.

#include "../../../Core/Public/Android/AndroidApplication.h"
#include "../../../Launch/Public/Android/AndroidJNI.h"
#include <android/log.h>

#define LOG_TAG "CameraLOG" // Used to print log with __android_log_print

__android_log_print will be used to print some logs from C++ to android side. You will need to use adb logcat in command line to catch them.

We will also add some variables 

int jniValue = 0; // We will increment it later to check if the Java communication worked
JNIEnv* javaEnvironment = NULL; // Will contain JNI when initialized
static jmethodID AndroidThunkJava_GetMessage; // Java function which will be called later


We finally obtain 

#if PLATFORM_ANDROID
	#include "../../../Core/Public/Android/AndroidApplication.h"
	#include "../../../Launch/Public/Android/AndroidJNI.h"
	#include <android/log.h>

	#define LOG_TAG "CameraLOG"

	int jniValue = 0;
	JNIEnv* javaEnvironment = NULL;
	static jmethodID AndroidThunkJava_GetMessage;
#endif




- Java Environment

We will create a initEnvironment() function

.h

int initEnvironment();


.cpp

int AJavaCommunication::initEnvironment() {
#if PLATFORM_ANDROID

	// Your code

#endif
return 0;
}

We will use an int as return value even if we don't manage it. It's just to show how to use JNI global variable (JNI_OK and JNI_ERR) to detect an error.

Call this function in BeginPlay()
void AJavaCommunication::BeginPlay()
{
	Super::BeginPlay();	
	initEnvironment();
}


Initialise the Java environment in  initEnvironment()

javaEnvironment = FAndroidApplication::GetJavaEnv();
if (!javaEnvironment) return JNI_ERR;


Now our environment is ready, we will initialise the java method to allow us to call it from C++

AndroidThunkJava_GetMessage = FJavaWrapper::FindMethod(javaEnvironment, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetMessage", "()V", false);

Let's talk about the arguments. In this example we added ()V to void argument and void return. 
We will write our arguments inside the parenthesis, and our return value outside.

If we want to add an int as argument, we will insert I inside the parenthesis (I)V, and if we want to return a string, we will use  ()Ljava/lang/String; for a String.
If you want a float array, followed by a boolean, you will use ([F;Z)V
As you saw previously, if you want to use an object you will use L + your object + ;

You will see a pretty good article in rgagnon.com which bring more explanation about these arguments.




We will finish this function with some logs

if (!AndroidThunkJava_GetMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("ERROR GEngine: AndroidThunkJava_GetMessage not found"));
	UE_LOG(LogTemp, Log, TEXT("ERROR UE_LOG: AndroidThunkJava_GetMessage not found"));
	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "ERROR __android_log_print: AndroidThunkJava_GetMessage not found");
	return JNI_ERR;
}

GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("Loading successful"));
return JNI_OK;


AddOnScreenDebugMessage print on your mobile screen (during 200 seconds in our exemple)
GEngine->AddOnScreenDebugMessage

UE_LOG print on your Unreal Output Log 
Display it in Window/Developper Tools/Output Log or click on the message on the bottom-right after clicking on the Launch button
UE_LOG

__android_log_print check your android log
Display it by using the adb logcat command (in command prompt) while you try to Launch your app.
__android_log_print

You can also print them in an external file with adb logcat > myFile.txt (it will be creating in the current command window path)
If adb doesn't work, you can find it in your android sdk path : android-sdk/platform-tools/adb.exe
Go there and open a command line in your explorer (shift + right click -> Open command window here) and use your command
adb.exe logcat

Or print all stack in a txt file
adb.exe logcat > C:/... yourPath .../myFile.txt





Full function

int AJavaCommunication::initEnvironment() {
#if PLATFORM_ANDROID

	javaEnvironment = FAndroidApplication::GetJavaEnv();
	if (!javaEnvironment) return JNI_ERR;

	AndroidThunkJava_GetMessage = FJavaWrapper::FindMethod(javaEnvironment, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetMessage", "()V", false);

	if (!AndroidThunkJava_GetMessage)
	{
		GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("ERROR GEngine: AndroidThunkJava_GetMessage not found"));
		UE_LOG(LogTemp, Log, TEXT("ERROR UE_LOG: AndroidThunkJava_GetMessage not found"));
		__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "ERROR __android_log_print: AndroidThunkJava_GetMessage not found");
		return JNI_ERR;
	}

	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, TEXT("Loading successful));
	return JNI_OK;

#endif
return 0;
}



- Tick


You should have a Tick definition in your h file

virtual void Tick(float DeltaTime) override;


Implement it in your cpp file if it's not already done, and add an Android condition

void AJavaCommunication::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	#if PLATFORM_ANDROID

		// Your code

	#endif
}




I'll execute my code example in a tick, but place your code in the place you want.
We will begin by checking if our environment and our method has been well loaded, and call our java method.

if (AndroidThunkJava_GetMessage && javaEnvironment) {
	FJavaWrapper::CallVoidMethod(javaEnvironment, FJavaWrapper::GameActivityThis, AndroidThunkJava_GetMessage);			

	if(jniValue > 0 && jniValue < 10)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::FromInt(jniValue));
}

If you want to return an int for example, you will need to use FjavaWrapper::CallIntMethod instead.  As I didn't tried to return a value, I won't give your more details.

We will increase jniValue just below. If it's working fine, we will be able to display some incrementing values. In every case, we won't spam to keep a look on our logs.




C++ method callable from Java


During the next part we will see how to call a C++ function from Java. Here is how to properly define our function.
We have to respect some rules :
- GetJNIData Will be the name of the method called. From Java will use GetJNIData(200);

Java and C++ don't use the same variable format. That's when all Java variables are converted to a jni variables, and to a C++ variables. 
Java int -> JNI jint -> C++ int




You can use differents type of variables 







You can also get an array with jbyteArray. I didn't test for the others.

- You have to keep Java_com_epicgames_ue4_GameActivity_ at the beginning name function

- You have to keep the 2 first variables as parameters



#if PLATFORM_ANDROID
extern "C" bool Java_com_epicgames_ue4_GameActivity_GetJNIData(JNIEnv* LocalJNIEnv, jobject LocalThiz, jint data)
{	
	jniValue += data; // data will contain 1
	return JNI_TRUE;
}
#endif




















JNI -> Java


Before starting this part, I recommand you to have this architecture in MyProject/Source











lib will contains our jar file, private contains our cpp files, public contains our h files. JarTuto and JarTutoGameModeBase are automatically generated when we created our C++ project. You will need to create a JarTuto_APL.xml file to create our Java code.

We will add a link to our xml in our build.cs file, in the constructor

if (Target.Platform == UnrealTargetPlatform.Android)
{
	string pluginPath = Utils.MakePathRelativeTo(ModuleDirectory, BuildConfiguration.RelativeEnginePath);
	AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", (pluginPath + "/JarTuto_APL.xml")));
        
}

- JarTuto_APL.xml

Create your xml file, in the same directory as our build.cs, with these initial parameters. I kept the camera access and differents imports to help you to know where and how to place your component access.


<?xml version="1.0" encoding="utf-8"?>
<!--ARToolKit plugin additions-->
<root xmlns:android="http://schemas.android.com/apk/res/android">
	 <!-- init section is always evaluated once per architecture -->
	 <init>
      <log text="JarTuto init"/>
	</init>
	
	<androidManifestUpdates>
		<addPermission android:name="android.permission.CAMERA"/>
		<addFeature android:name="android.hardware.camera"/>
		<addFeature android:name="android.hardware.camera.autofocus"/>
	</androidManifestUpdates>
  
	<resourceCopies>
	</resourceCopies>  

  <!-- optional additions to the GameActivity imports in GameActivity.java -->
	<gameActivityImportAdditions>
		<insert>
import android.widget.Toast;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.graphics.SurfaceTexture;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import java.util.List; 
import java.io.IOException;
import android.util.Log;
		</insert>
	</gameActivityImportAdditions>
	
	<gameActivityClassAdditions>
		<insert>


 
		</insert>
	</gameActivityClassAdditions>

	<!-- optional additions to GameActivity onCreate in GameActivity.java -->
	<gameActivityOnCreateAdditions>
		<insert>
      // Java code
		</insert>
	</gameActivityOnCreateAdditions>
</root>
Now we will complete our xml file to add our java function
Open MyProject_APL.xml

We will insert our code between these tags

<gameActivityClassAdditions>
	<insert>

		// Your java code

	</insert>
</gameActivityClassAdditions>



Add a signature of our C++ function which will be called

public native boolean GetJNIData(int data);


After that, we will add our GetMessage() java function

public void AndroidThunkJava_GetMessage() 
{      
	try
	{
		System.out.println("Java JNI Worked !");

		int testJNIValue = new TestJNI().testJNI(); // 250

      		if(testJNIValue == 250)
        		GetJNIData(1);
	}
	catch (Exception e)
	{
		Log.debug("Failed with exception " + e.getMessage());
	}
}

We instantiate a TestJNI class which is the class contained in our jar file. This class has a testJNI method which return 250. If we successfully return this value, we will call our C++ function to increment our jniValue.




We managed to communicate with our java code and returned a value by calling a C++ function. We can access to different functionalities of our android device. But another problem stay. What will you do if your developper team created an awesome functionality in Java langage ? I'm not sure you want to have everything in your xml file.

That's why we will see how to integrate a jar file to our code.




Java -> Jar


Before starting, I have some things to explain about how to have a good working jar. When I write this tutorial, the actual jdk version is 1.8.0_121 and CodeWorks make us installing the 1.8.0_77
Even by trying a lot, I wasn't able to make the 1.8 version working. That's why I will recommand you to install the jdk 1.7 at the Oracle Archive.
If you want to compile with eclipse (or another software) you may prefer download the jre.


- Creating a Jar file


I will give you 3 ways to create your jar file. Before, I will ask you to place all your .java in a package named in our example: comPackage. Do not leave them in a default package (right click on your package -> new -> package and move your class inside).


- 1 - Eclipse


You have to set your jre first. Go to Project -> Properties -> Java Build Path -> Libraries tab, click on your JRE, and edit (on the right). Here you should be able to set your installation.

Select your project, File -> Export -> Java/JAR file -> Next -> Select your export destination and click on the Finish button


- 2 - Command Line – Java Home


After installing CodeWorks, your Java_Home may point to this folder, and so the jdk used by Unreal. To find your Environment Variables (in Window 7) : Click on the Start button -> Right click on Computer -> Properties -> Advanced system settings -> Environment Variables... and set the JAVA_HOME variable to your jdk 1.7 repertory (not the bin folder).

Now you can find your .java file and compile them. Use Shift + Righ click to display the "Open command window here" option, and click on it. Now use the command line: javac fileName.java  or javac *.java  depending of your needs. 


- 3 - Commande Line – No configuration need

The best method to rapidly test. Go to your jdk, and open its bin folder. Use Shift + Righ click to display the "Open command window here" option, and click on it. Now use the command line : 
javac.exe pathOfYourJavaFile.java  It will compile your file to the pathOfYourJavaFile.java directory.


If you used the 2 or 3 methods, you will need to create manually your jar file. For that, open a command line in your bin folder, and use the command  jar cf myJar.jar comPackage/TestJNI.class 

- Checking your class version

Now you have your class file. We will need to create a java project to check if we used the good jdk version to compile. Create a project and a main class. Copy this script and give it the link of your class file.


String filename = "D:/my/path/folder/TestJNI.class"; // Change by your class path
    	
    	
DataInputStream in = new DataInputStream (new FileInputStream(filename));

int magic = in.readInt();
if(magic != 0xcafebabe) {
	System.out.println(filename + " is not a valid class!");;
}
int minor = in.readUnsignedShort();
int major = in.readUnsignedShort();
System.out.println(filename + ": " + major + " . " + minor);
in.close();


You should obtain something like that
D:/my/path/folder/TestJNI.class: 51 . 0


Here is an array to interpret these values

 major  minor Java platform version 
 45       3           1.0
 45       3           1.1
 46       0           1.2
 47       0           1.3
 48       0           1.4
 49       0           1.5
 50       0           1.6
 51       0           1.7
 52       0           1.8


If you obtained 51 . 0 that mean you well compiled your java file with your 1.7 jdk















Importing your jar file in your _APL.xml

Add your myJar.jar file in MyProject/Source/MyProject/lib


Now open MyProject_APL.xml


Proguard is used to modify the name of your class and protect your code. That's why we may need to add an exception to our proguard. Like that we will still being able to call our class by its name.

I'm not sure when we need to use it because it's working for me even without it. If you have a problem to use your class, test to configure proguard.
Here are some hint for you to test and search, they are not a working solution and are not tested, it's just for you to better search for your own solution.

  <proguardAdditions>
    <insert>
      
	// Your proguard code

    </insert>
  </proguardAdditions>




We will need to duplicate our jar to the intermediate directory. Sometimes we delete our Binaries, Intermediate and Saved folder to solve a problem. That's why it's better to create the copy directly in you xml code than duplicated manually the jar file. (MyProject\Intermediate\Android\APK\libs)

PluginDir is the directory where you have your _APL.xml file that you defined in build.cs when you had the AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", (pluginPath + "/JarTuto_APL.xml")));
BuildDir is the APK directory in your intermediate folder


  <resourceCopies>

    <copyFile src="$S(PluginDir)/lib/myJar.jar" dst="$S(BuildDir)/libs/myJar.jar" />
  
  </resourceCopies>



Add the import ligne just below the others import in the gameActivityImportAdditions/insert

import comPackage.TestJNI;







Method not found

It can happens that your java method is not found or any other problem. That mean your xml is not read fine. You can try to delete the Binaries, Intermediate and Saved folder, right-click on your MyProject.uproject and click on Generate Visual Studio project files. Check if your xml content is still here, rebuild, compile launch again. Keep a look on your Unreal Output Logs, and also your adb logcat.




