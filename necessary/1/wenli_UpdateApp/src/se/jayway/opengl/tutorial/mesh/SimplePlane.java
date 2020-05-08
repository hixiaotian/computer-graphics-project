package se.jayway.opengl.tutorial.mesh;

public class SimplePlane extends Mesh {
	/**
	 * Create a plane with a default with and height of 1 unit.
	 */
	public SimplePlane() {
		this(1, 1);
	}

	/**
	 * Create a plane.
	 * 
	 * 
	 */
	public SimplePlane(float width, float height) {
		// Mapping coordinates for the vertices
		// float m;
		float textureCoordinates[] = { 0.0f, 1.0f, 1.0f, 1.0f, //
				0.0f, 0.0f, //
				1.0f, 0.0f, //
		};
		// for (m = 0; m < 1.0f; m = m + 0.05f) {
		// float tetureColores[] = { m, 0, 0, 1.0f, 0, 0, m, 1.0f, 0, m, 0.2f,
		// 1.0f, m, 0.5f, 0, 1.0f,
		//
		// };
		// try {
		// Thread.sleep(200);
		// } catch (InterruptedException e) {
		// System.out.println("颜色更换不成功");
		// }
		// setColors(tetureColores);
		// }

		short[] indices = new short[] { 0, 1, 2, 1, 3, 2 };

		float[] vertices = new float[] { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f,
				-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f };

		setIndices(indices);
		setVertices(vertices);
		setTextureCoordinates(textureCoordinates);
	}
}
