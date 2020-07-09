package com.eland.plugins;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.jiransoft.mdm.library.MDMLib;
import com.jiransoft.mdm.library.Services.OnMangobananaCompleteListener;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.support.v4.app.ActivityCompat;
import android.telephony.TelephonyManager;
import android.util.Log;

public class MDMPlugin extends CordovaPlugin implements OnMangobananaCompleteListener {

	private static final int REQUEST_READ_PHONE_STATE_PERMISSION	= 1000;
	private String LOG_TAG = "MDMPlugin";
	private String CHECK_APP = "check_app";
	private String authServer = "mdm30ssl.eland.co.kr";
	private String authServerPort = "44300";
	private String companyCode = "17121500";
	private Context context;
	private CallbackContext statusCallbackContext;

	private static MDMLib mMDMLib = null;
	private static Handler mHandler = null;

	@Override
	public void initialize(CordovaInterface cordova, CordovaWebView webView) {
		super.initialize(cordova, webView);
		this.statusCallbackContext = null;
	}

	@Override
	public boolean execute(String action, JSONArray args,
						   final CallbackContext callbackContext) throws JSONException {



		context = this.cordova.getActivity().getApplicationContext();
		this.statusCallbackContext = callbackContext;


		if(action.equals(CHECK_APP)) {

			Log.i(LOG_TAG, "check_app");
			Activity activity = cordova.getActivity(); //(Activity)context; //this.cordova.getActivity(); //(Activity)context;
			mMDMLib = MDMLib.getInstance(activity);
			//mMDMLib.setDebugMode(true);

			mHandler = new Handler();
			mMDMLib.setOnMangobananaCompleteListener(this);
			//mMDMLib.startCurrentStatusCheckService(activity, null);


			// OS 6.0 이상인 경우 전화 권한 활성화 유도 (단말의 고유값 추출하는데 사용)
			if (Build.VERSION_CODES.M <= Build.VERSION.SDK_INT) {
				if (PackageManager.PERMISSION_GRANTED != ActivityCompat.checkSelfPermission(context, Manifest.permission.READ_PHONE_STATE)) {
					ActivityCompat.requestPermissions(activity, new String[]{Manifest.permission.READ_PHONE_STATE}, REQUEST_READ_PHONE_STATE_PERMISSION);
				}
			}

			mMDMLib.mangobanana(activity, mHandler, companyCode, authServer, authServerPort, "");
			return true;

		}

		return super.execute(action, args, callbackContext);
	}

	private String getLocale(Context context) {
		return context.getResources().getConfiguration().locale.getCountry();
	}

	/*
	private String getDeviceId (Context context) {
		TelephonyManager tm = (TelephonyManager)context.getSystemService(context.TELEPHONY_SERVICE);
		return tm.getDeviceId();
	}
	*/

	private String getPackageName(Context context) {
		return context.getPackageName();
	}

	private String getMacAddress() {
		try {
			List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface nif : all) {
				if (!nif.getName().equalsIgnoreCase("wlan0")) continue;

				byte[] macBytes = nif.getHardwareAddress();
				if (macBytes == null) {
					return "";
				}

				StringBuilder res1 = new StringBuilder();
				for (byte b : macBytes) {
					res1.append(String.format("%02X:",b));
				}

				if (res1.length() > 0) {
					res1.deleteCharAt(res1.length() - 1);
				}
				return res1.toString();
			}
		} catch (Exception ex) {
		}
		return "02:00:00:00:00:00";
	}

	@Override
	public void onMangobananaComplete(String code, String message) {

		String packageName = getPackageName(context);
		String locale = getLocale(context);

		Log.i(LOG_TAG, "code: " + code);
		Log.d(LOG_TAG, "ID: " + message);

		final JSONObject status = new JSONObject();
		try{
			status.put("packageName", packageName);
			status.put("locale", locale);
			status.put("codeKey", code);
			status.put("message", message);
		}catch(Exception e){
		}

		PluginResult pluginResult = new PluginResult(PluginResult.Status.OK, status);
		pluginResult.setKeepCallback(true);
		this.statusCallbackContext.sendPluginResult(pluginResult);

	}
}

