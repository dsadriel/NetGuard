#include "input/mouse_keyboard_callbacks.hpp"

// Função callback chamada sempre que o usuário aperta algum dos botões do mouse
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	g_NetGuard.handleMouseClick(window, button, action, mods);
}

// Função callback chamada sempre que o usuário movimentar o cursor do mouse em
// cima da janela OpenGL.
void CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	g_NetGuard.handleMouseMovement(window, xpos, ypos);
}

// Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
// void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     // Atualizamos a distância da câmera para a origem utilizando a
//     // movimentação da "rodinha", simulando um ZOOM.
//     g_NetGuard.cameraDistance -= 0.1f*yoffset;

//     // Uma câmera look-at nunca pode estar exatamente "em cima" do ponto para
//     // onde ela está olhando, pois isto gera problemas de divisão por zero na
//     // definição do sistema de coordenadas da câmera. Isto é, a variável abaixo
//     // nunca pode ser zero. Versões anteriores deste código possuíam este bug,
//     // o qual foi detectado pelo aluno Vinicius Fraga (2017/2).
//     const float verysmallnumber = numeric_limits<float>::epsilon();
//     if (g_NetGuard.cameraDistance < verysmallnumber)
//         g_NetGuard.cameraDistance = verysmallnumber;
// }

// Definição da função que será chamada sempre que o usuário pressionar alguma tecla do teclado.
// Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod) {

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_P:
			g_NetGuard.nexStage();
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		default:
			break;
		}
	}
}