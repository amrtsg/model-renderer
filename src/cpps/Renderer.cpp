#include <Renderer.h>

//Render all objects in scene
void Renderer::renderScene(Light light, vector<Model> entities, glm::mat4 view, glm::mat4 projection, Camera camera, glm::vec4 clipPlane) {
	for (int i = 0; i < entities.size(); i++) {
		//DRAW .OBJ__________________________________________________________________
		entities[i].getShader()->use();
		entities[i].getShader()->setUniforms(entities[i].getModelMatrix(), view, projection);
		entities[i].getShader()->loadClipPlane(clipPlane);
		entities[i].draw(entities[i].getShader());
	}

}