package se.jayway.opengl.tutorial;

import se.jayway.opengl.tutorial.mesh.SimplePlane;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {

	@SuppressLint("NewApi")
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Make the windows into full screen mode.
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// 创建视图
		GLSurfaceView view = new GLSurfaceView(this);

		OpenGLRenderer renderer = new OpenGLRenderer();
		view.setRenderer(renderer);
		setContentView(view);

		// Create a new plane.
		SimplePlane plane = new SimplePlane(1, 1);

		// Move and rotate the plane.
		// plane.z = 1.7f;
		// plane.rx = -65;

		Bitmap bitmap1 = BitmapFactory.decodeResource(getResources(),
				R.drawable.jay);
		Bitmap bitmap2 = BitmapFactory.decodeResource(getResources(),
				R.drawable.ic_launcher);
		//
		// List<Bitmap> list = new ArrayList<Bitmap>();
		// list.add(bitmap1);
		// list.add(bitmap2);
		// MyThread m = new MyThread();

		// new Thread(m).start();

		// Load the texture.
	plane.loadBitmap(bitmap1);
		plane.loadBitmap(bitmap2);

		// Add the plane to the renderer.
		renderer.addMesh(plane);

	}

	// @SuppressLint("HandlerLeak")
	// class MyHandler extends Handler {
	//
	// public MyHandler() {
	//
	// }
	//
	// @SuppressLint("HandlerLeak")
	// public MyHandler(Looper L) {
	//
	// super(L);
	//
	// }
	//
	// // 子类必须重写此方法,接受数据
	//
	// @Override
	// public void handleMessage(Message msg) {
	//
	// // TODO Auto-generated method stub
	//
	// // 此处更新UI
	// switch (msg.arg1) {
	// case 0:
	// bitmap = BitmapFactory.decodeResource(getResources(),
	// R.drawable.jay);
	// }
	// super.handleMessage(msg);
	// }
	//
	// }
	//
	// class MyThread implements Runnable {
	//
	// public void run() {
	// int i = 0;
	//
	// try {
	//
	// Thread.sleep(1000);
	//
	// } catch (InterruptedException e) {
	//
	// // TODO Auto-generated catch block
	//
	// e.printStackTrace();
	//
	// }
	// bitmap = BitmapFactory.decodeResource(getResources(),
	// R.drawable.ic_launcher);
	//
	// Message msg = new Message();
	//
	// msg.arg1 = i;
	// MyHandler myhandler = new MyHandler();
	//
	// myhandler.sendMessage(msg); // 向Handler发送消息,更新UI
	//
	// }

	// }

}
