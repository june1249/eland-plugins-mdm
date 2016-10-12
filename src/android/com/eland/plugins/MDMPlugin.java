package com.eland.plugins;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.jiransoft.mdm.library.MDMLib;

import android.content.Context;
import android.telephony.TelephonyManager;
import android.util.Log;

public class MDMPlugin extends CordovaPlugin {
	
	private String CHECK_APP = "check_app";
	private String authServer = "mdmssl.eland.co.kr:44400";
	private String companyCode = "11121400";
	private Context context;
	@Override
	public boolean execute(String action, JSONArray args,
			final CallbackContext callbackContext) throws JSONException {
		
		context = this.cordova.getActivity().getApplicationContext();
		
		if(action.equals(CHECK_APP)) {
			final JSONObject status = new JSONObject();
			this.cordova.getActivity().runOnUiThread(new Runnable() {

				@Override
				public void run() {
					
					String deviceID = "";					
					String imei = getDeviceId(context);					
					if(imei == null){
						deviceID = null;
					}else{
						deviceID = imei;
					}					
					String packageName = getPackageName(context);
					String locale = getLocale(context);
					
					Log.i("MDM", "deviceID : " + deviceID);
					Log.i("MDM", "packageName : " + packageName);
					Log.i("MDM", "locale : " + locale);
					
					HashMap<String, String> appAuthentication = MDMLib.appAuthentication(context, authServer, companyCode, "1111", deviceID);
					
					String codeKey = appAuthentication.get(MDMLib.CODE_KEY);
					String message = appAuthentication.get(MDMLib.MESSAGE_KEY);
					
					Log.i("MDM", "codeKey : " + codeKey);
					Log.i("MDM", "message : " + message);
					
					try{						
						status.put("packageName", packageName);
						status.put("locale", locale);
						status.put("codeKey", codeKey);
						status.put("message", message);						
					}catch(Exception e){
						
					}
					
					PluginResult pluginResult = new PluginResult(PluginResult.Status.OK, status);
		            pluginResult.setKeepCallback(true);
		            callbackContext.sendPluginResult(pluginResult);					
				}				
			});
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

}
