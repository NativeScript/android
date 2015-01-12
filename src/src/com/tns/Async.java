package com.tns;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;

public class Async
{
	public interface CompleteCallback {
		void onComplete(Object result, Object context);
	}
	
	public static void DownloadImage(String url, CompleteCallback callback, Object context){
		new ImageDownloader(callback, context).execute(url);
	}
	
	static class ImageDownloader extends AsyncTask<String, Void, Bitmap> {
		private CompleteCallback callback;
		private Object context;
		
		public ImageDownloader(CompleteCallback callback, Object context){
			this.callback = callback;
			this.context = context;
		}
		
		protected Bitmap doInBackground(String... params) {
			InputStream stream = null;
			try {
				stream = new java.net.URL(params[0]).openStream();
				Bitmap bmp = BitmapFactory.decodeStream(stream);
				return bmp;
			} catch (MalformedURLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			} finally {
				if(stream != null){
					// TODO: WTF??? Try in finally - JAVA Rules!
					try
					{
						stream.close();
					}
					catch (IOException e)
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		}
		
		protected void onPostExecute(final Bitmap result) {
			this.callback.onComplete(result, this.context);
	    }
	}
	
	// static class 
}
