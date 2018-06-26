package com.eland.plugins;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.HashMap;
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

import android.content.Context;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Log;

public class MDMPlugin extends CordovaPlugin implements OnMangobananaCompleteListener {

	private String LOG_TAG = "MDMPlugin";
	private String CHECK_APP = "check_app";
	private String authServer = "mdm30ssl.eland.co.kr";
	private String authseverPort = "44300";
	private String companyCode = "17121500";
	private Context context;
	private Handler mdmHandler;
	private CallbackContext statusCallbackContext;

	@Override
	public void initialize(CordovaInterface cordova, CordovaWebView webView) {
		super.initialize(cordova, webView);
		Log.i(LOG_TAG, "initialize");
		this.statusCallbackContext = null;
		MDMLib.setOnMangobananaCompleteListener(this);
	}

	@Override
	public boolean execute(String action, JSONArray args,
			final CallbackContext callbackContext) throws JSONException {

		context = this.cordova.getActivity().getApplicationContext();

		if(action.equals(CHECK_APP)) {
			this.statusCallbackContext = callbackContext;
			mdmHandler = new Handler();
			MDMLib.mangobanana(cordova.getActivity(), mdmHandler, companyCode, authServer, authseverPort, "");
			return true;
		}

		return super.execute(action, args, callbackContext);
	}

	private String getLocale(Context context) {
		return context.getResources().getConfiguration().locale.getCountry();
	}

	private String getDeviceId (Context context) {
		TelephonyManager tm = (TelephonyManager)context.getSystemService(context.TELEPHONY_SERVICE);
		return tm.getDeviceId();
	}

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
