package com.epicgames.ue4;

import com.YourCompany.JarTuto.OBBDownloaderService;
import com.YourCompany.JarTuto.DownloaderActivity;


public class DownloadShim
{
	public static OBBDownloaderService DownloaderService;
	public static DownloaderActivity DownloadActivity;
	public static Class<DownloaderActivity> GetDownloaderType() { return DownloaderActivity.class; }
}

