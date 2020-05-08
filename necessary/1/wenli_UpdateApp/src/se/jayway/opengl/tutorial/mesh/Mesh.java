package se.jayway.opengl.tutorial.mesh;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.opengles.GL10;

import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class Mesh {
	// Our vertex buffer.
	private FloatBuffer mVerticesBuffer = null;

	// Our index buffer.
	private ShortBuffer mIndicesBuffer = null;

	// Our UV texture buffer.
	private FloatBuffer mTextureBuffer; // New variable.

	// Our texture id.
	private int mTextureId = -1; // New variable.

	// The bitmap we want to load as a texture.
	private Bitmap mBitmap; // New variable.

	// Indicates if we need to load the texture.
	private boolean mShouldLoadTexture = false; // New variable.

	// The number of indices.
	private int mNumOfIndices = -1;

	// Flat Color
	private final float[] mRGBA = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };

	// Smooth Colors
	private FloatBuffer mColorBuffer = null;

	// Translate params.
	public float x = 0;

	public float y = 0;

	public float z = 0;

	// Rotate params.
	public float rx = 0;

	public float ry = 0;

	public float rz = 0;
	private float m = 0;

	// private static float[]tetureColores;
	/**
	 * Render the mesh.
	 * 
	 * @param gl
	 *            the OpenGL context to render to.
	 */
	// static {
	// for(float k=0;k<1.0f;k=k+0.05f){
	// tetureColores[k]=
	//
	// }
	//
	// }
	public void draw(GL10 gl) {

		// float m;
		// 处理平滑着色的方法
		// for (m = 0; m < 1.0f; m = m + 0.05f) {
		// float tetureColores[] = { m, 0, 0, 1.0f, 0, 0, m, 1.0f, 0, m, 0.2f,
		// 1.0f, m, 0.5f, 0, 1.0f,
		//
		// };

		float tetureColores[] = { m, 0, 0, 1.0f, 0, 0, m, 1.0f, 0, m, 0.2f,
				1.0f, m, 0.5f, 0, 1.0f, };
		m = m + 0.05f;
		try {
			Thread.sleep(200);
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if (m >= 1.0f)
			m = 0;
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			System.out.println("颜色更换不成功");
		}
		setColors(tetureColores);

		gl.glFrontFace(GL10.GL_CCW);
		// Enable face culling.
		gl.glEnable(GL10.GL_CULL_FACE);
		// What faces to remove with the face culling.
		gl.glCullFace(GL10.GL_BACK);
		// Enabled the vertices buffer for writing and to be used during
		// rendering.
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		// Specifies the location and data format of an array of vertex
		// coordinates to use when rendering.
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, mVerticesBuffer);

		// 处理平滑着色的方法
		if (mColorBuffer != null) {
			// Enable the color array buffer to be used during rendering.
			gl.glEnableClientState(GL10.GL_COLOR_ARRAY);
			gl.glColorPointer(4, GL10.GL_FLOAT, 0, mColorBuffer);
		}
		// gl.glColor4f(0.5f, 0.5f, 1.0f, 1.0f);

		// 将纹理映射到矩形区域
		if (mShouldLoadTexture) {
			// 加载纹理
			loadGLTexture(gl);
			mShouldLoadTexture = false;
		}
		if (mTextureId != -1 && mTextureBuffer != null) {
			gl.glEnable(GL10.GL_TEXTURE_2D);
			// 开启纹理
			gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);

			// 将纹理坐标绑定要映射的物体上
			gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mTextureBuffer);
			gl.glBindTexture(GL10.GL_TEXTURE_2D, mTextureId);
		}

		// 移动图片
		gl.glTranslatef(x, y, z);
		// 旋转图片
		// gl.glRotatef(rx, 1, 0, 0);
		// gl.glRotatef(ry, 0, 1, 0);
		// gl.glRotatef(rz, 0, 0, 1);

		// Point out the where the color buffer is.
		gl.glDrawElements(GL10.GL_TRIANGLES, 6, GL10.GL_UNSIGNED_SHORT,
				mIndicesBuffer);
		// Disable the vertices buffer.
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);

		// New part...
		if (mTextureId != -1 && mTextureBuffer != null) {
			gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		}

		// Disable face culling.
		gl.glDisable(GL10.GL_CULL_FACE);
	}

	/**
	 * Set the vertices.
	 * 
	 * @param vertices
	 */
	protected void setVertices(float[] vertices) {
		// a float is 4 bytes, therefore we multiply the number if
		// vertices with 4.
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		mVerticesBuffer = vbb.asFloatBuffer();
		mVerticesBuffer.put(vertices);
		mVerticesBuffer.position(0);
	}

	/**
	 * Set the indices.
	 * 
	 * @param indices
	 */
	protected void setIndices(short[] indices) {
		// short is 2 bytes, therefore we multiply the number if
		// vertices with 2.
		ByteBuffer ibb = ByteBuffer.allocateDirect(indices.length * 2);
		ibb.order(ByteOrder.nativeOrder());
		mIndicesBuffer = ibb.asShortBuffer();
		mIndicesBuffer.put(indices);
		mIndicesBuffer.position(0);
		mNumOfIndices = indices.length;
	}

	/**
	 * Set the texture coordinates.
	 * 
	 * @param textureCoords
	 */
	protected void setTextureCoordinates(float[] textureCoords) { // New
																	// function.
		// float is 4 bytes, therefore we multiply the number if
		// vertices with 4.
		ByteBuffer byteBuf = ByteBuffer
				.allocateDirect(textureCoords.length * 4);
		byteBuf.order(ByteOrder.nativeOrder());
		mTextureBuffer = byteBuf.asFloatBuffer();
		mTextureBuffer.put(textureCoords);
		mTextureBuffer.position(0);
	}

	/**
	 * Set one flat color on the mesh.
	 * 
	 * @param red
	 * @param green
	 * @param blue
	 * @param alpha
	 */
	protected void setColor(float red, float green, float blue, float alpha) {
		mRGBA[0] = red;
		mRGBA[1] = green;
		mRGBA[2] = blue;
		mRGBA[3] = alpha;
	}

	/**
	 * Set the colors
	 * 
	 * @param colors
	 */
	protected void setColors(float[] colors) {
		// float has 4 bytes.
		ByteBuffer cbb = ByteBuffer.allocateDirect(colors.length * 4);
		cbb.order(ByteOrder.nativeOrder());
		mColorBuffer = cbb.asFloatBuffer();
		mColorBuffer.put(colors);
		mColorBuffer.position(0);
	}

	/**
	 * Set the bitmap to load into a texture.
	 * 
	 * @param bitmap
	 */
	public void loadBitmap(Bitmap bitmap) { // New function.
		this.mBitmap = bitmap;
		mShouldLoadTexture = true;
	}

	// 初始化一个纹理
	private void loadGLTexture(GL10 gl) { // New function
		// Generate one texture pointer...
		int[] textures = new int[2];
		// 1表示纹理个数，textures表示纹理的代号，0表示纹理索引号
		gl.glGenTextures(1, textures, 0);
		mTextureId = textures[0];

		// 绑定纹理到纹理目标上
		gl.glBindTexture(GL10.GL_TEXTURE_2D, mTextureId);

		// Create Nearest Filtered Texture
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MIN_FILTER,
				GL10.GL_LINEAR);
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_MAG_FILTER,
				GL10.GL_LINEAR);

		// Different possible texture parameters, e.g. GL10.GL_CLAMP_TO_EDGE
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_S,
				GL10.GL_REPEAT);
		gl.glTexParameterf(GL10.GL_TEXTURE_2D, GL10.GL_TEXTURE_WRAP_T,
				GL10.GL_CLAMP_TO_EDGE);

		// Use the Android GLUtils to specify a two-dimensional texture image
		// from our bitmap
		GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, mBitmap, 0);
	}
}
