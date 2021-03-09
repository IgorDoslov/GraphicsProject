#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Plane.h"
#include "PhysicsScene.h"
#include <vector>
#include "Box.h"
#include "Sphere.h"
#include "Spring.h"


PhysicsProjectApp::PhysicsProjectApp() {

}

PhysicsProjectApp::~PhysicsProjectApp() {

}

bool PhysicsProjectApp::startup() {

	// Increases the 2D line count to maximise the number of objects
	// we can draw.
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"

	m_font = new aie::Font("bin/font/consolas.ttf", 32);
	m_ballFont = new aie::Font("bin/font/consolas.ttf", 18);

	//m_tableTexture = new aie::Texture("./textures/Pool_Table_Type_1_BG Wide.png");
	//m_testTex = new aie::Texture("./textures/grass.png");

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0, 0));


	// Lower the value, the more accurate the simulation will be;
	// but it will increase the processing time required. If it
	// is too high it cause the sim to stutter and reduce stability.

	m_physicsScene->SetTimeStep(0.01f);

	DrawTable();
	DrawBalls();
	DrawPockets();

	return true;
}

void PhysicsProjectApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsProjectApp::update(float deltaTime) {
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();


	PoolGame(input);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

}


void PhysicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	aie::Application::setBackgroundColour(0, 0.55f, 0.2f);

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	//aie::Gizmos::add2DLine(glm::vec2(0, 0), glm::vec2(0, 40), glm::vec4(1, 0, 0, 1));

	// X axis = -100 to 100, Y axis = -56.25 to 56.25
	aie::Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());//(glm::ortho<float>(-m_extents, m_extents,
		//-m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.f, 1.f));

	char fps[32];
	sprintf_s(fps, 32, "Power: %i", getFPS());
	m_2dRenderer->drawText(m_font, m_text.c_str(), 380, 730);


	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);
	m_2dRenderer->drawText(m_font, "Press r to reset", 740, 0);
	m_2dRenderer->drawText(m_font, "Player 1", 20, 110);
	m_2dRenderer->drawText(m_font, "Player 2", 20, 60);
	m_2dRenderer->drawText(m_ballFont, "1", yellowSolid1->GetPosition().x - 5, yellowSolid1->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "2", blueSolid2->GetPosition().x - 5, blueSolid2->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "3", redSolid3->GetPosition().x - 5, redSolid3->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "4", purpleSolid4->GetPosition().x - 5, purpleSolid4->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "5", orangeSolid5->GetPosition().x - 5, orangeSolid5->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "6", greenSolid6->GetPosition().x - 5, greenSolid6->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "7", maroonSolid7->GetPosition().x - 5, maroonSolid7->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "8", blackBall8->GetPosition().x - 5, blackBall8->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "9", yellowStripe9->GetPosition().x - 5, yellowStripe9->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "10", blueStripe10->GetPosition().x - 10, blueStripe10->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "11", redStripe11->GetPosition().x - 10, redStripe11->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "12", purpleStripe12->GetPosition().x - 10, purpleStripe12->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "13", orangeStripe13->GetPosition().x - 10, orangeStripe13->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "14", greenStripe14->GetPosition().x - 10, greenStripe14->GetPosition().y - 5, 1);
	m_2dRenderer->drawText(m_ballFont, "15", maroonStripe15->GetPosition().x - 10, maroonStripe15->GetPosition().y - 5, 1);


	//#pragma region DrawSpritesForBalls
	//
	//m_2dRenderer->drawSprite(m_whiteBallTexture, whiteBall->GetPosition().x, whiteBall->GetPosition().y, whiteBall->GetRadius() * 2, whiteBall->GetRadius() * 2, whiteBall->GetRotation());
	//	m_2dRenderer->drawSprite(m_blackBall8Texture, blackBall8->GetPosition().x, blackBall8->GetPosition().y, blackBall8->GetRadius() * 2, blackBall8->GetRadius() * 2, blackBall8->GetRotation());
	//	m_2dRenderer->drawSprite(m_yellowSolid1Texture, yellowSolid1->GetPosition().x, yellowSolid1->GetPosition().y, yellowSolid1->GetRadius() * 2, yellowSolid1->GetRadius() * 2, yellowSolid1->GetRotation());
	//	m_2dRenderer->drawSprite(m_blueSolid2Texture, blueSolid2->GetPosition().x, blueSolid2->GetPosition().y, blueSolid2->GetRadius() * 2, blueSolid2->GetRadius() * 2, blueSolid2->GetRotation());
	//	m_2dRenderer->drawSprite(m_redSolid3Texture, redSolid3->GetPosition().x, redSolid3->GetPosition().y, redSolid3->GetRadius() * 2, redSolid3->GetRadius() * 2, redSolid3->GetRotation());
	//	m_2dRenderer->drawSprite(m_purpleSolid4Texture, purpleSolid4->GetPosition().x, purpleSolid4->GetPosition().y, purpleSolid4->GetRadius() * 2, purpleSolid4->GetRadius() * 2, purpleSolid4->GetRotation());
	//	m_2dRenderer->drawSprite(m_orangeSolid5Texture, orangeSolid5->GetPosition().x, orangeSolid5->GetPosition().y, orangeSolid5->GetRadius() * 2, orangeSolid5->GetRadius() * 2, orangeSolid5->GetRotation());
	//	m_2dRenderer->drawSprite(m_greenSolid6Texture, greenSolid6->GetPosition().x, greenSolid6->GetPosition().y, greenSolid6->GetRadius() * 2, greenSolid6->GetRadius() * 2, greenSolid6->GetRotation());
	//	m_2dRenderer->drawSprite(m_maroonSolid7Texture, maroonSolid7->GetPosition().x, maroonSolid7->GetPosition().y, maroonSolid7->GetRadius() * 2, maroonSolid7->GetRadius() * 2, maroonSolid7->GetRotation());
	//	m_2dRenderer->drawSprite(m_yellowStripe9Texture, yellowStripe9->GetPosition().x, yellowStripe9->GetPosition().y, yellowStripe9->GetRadius() * 2, yellowStripe9->GetRadius() * 2, yellowStripe9->GetRotation());
	//	m_2dRenderer->drawSprite(m_blueStripe10Texture, blueStripe10->GetPosition().x, blueStripe10->GetPosition().y, blueStripe10->GetRadius() * 2, blueStripe10->GetRadius() * 2, blueStripe10->GetRotation());
	//	m_2dRenderer->drawSprite(m_redStripe11Texture, redStripe11->GetPosition().x, redStripe11->GetPosition().y, redStripe11->GetRadius() * 2, redStripe11->GetRadius() * 2, redStripe11->GetRotation());
	//	m_2dRenderer->drawSprite(m_purpleStripe12Texture, purpleStripe12->GetPosition().x, purpleStripe12->GetPosition().y, purpleStripe12->GetRadius() * 2, purpleStripe12->GetRadius() * 2, purpleStripe12->GetRotation());
	//	m_2dRenderer->drawSprite(m_orangeStripe13Texture, orangeStripe13->GetPosition().x, orangeStripe13->GetPosition().y, orangeStripe13->GetRadius() * 2, orangeStripe13->GetRadius() * 2, orangeStripe13->GetRotation());
	//	m_2dRenderer->drawSprite(m_greenStripe14Texture, greenStripe14->GetPosition().x, greenStripe14->GetPosition().y, greenStripe14->GetRadius() * 2, greenStripe14->GetRadius() * 2, greenStripe14->GetRotation());
	//	m_2dRenderer->drawSprite(m_maroonStripe15Texture, maroonStripe15->GetPosition().x, maroonStripe15->GetPosition().y, maroonStripe15->GetRadius() * 2, maroonStripe15->GetRadius() * 2, maroonStripe15->GetRotation());
	//
	//#pragma endregion

		// done drawing sprites
	m_2dRenderer->end();
}


#pragma region Setup Functions

void PhysicsProjectApp::DrawTable()
{
	// bottom left
	m_bottomLeftBox = new Box(glm::vec2(300, 206), glm::vec2(0, 0), 0, 10, 186, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_bottomLeftBox2 = new Box(glm::vec2(300, 156), glm::vec2(0, 0), 0, 10, 267, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	// bottom right
	m_bottomRightBox = new Box(glm::vec2(725, 206), glm::vec2(0, 0), 0, 10, 186, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_bottomRightBox2 = new Box(glm::vec2(725, 156), glm::vec2(0, 0), 0, 10, 267, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	// top left
	m_topLeftBox = new Box(glm::vec2(725, 657), glm::vec2(0, 0), 0, 10, 186, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_topLeftBox2 = new Box(glm::vec2(725, 707), glm::vec2(0, 0), 0, 10, 267, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	// top right
	m_topRightBox = new Box(glm::vec2(300, 657), glm::vec2(0, 0), 0, 10, 186, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_topRightBox2 = new Box(glm::vec2(300, 707), glm::vec2(0, 0), 0, 10, 267, 4,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	// right side
	m_rightSideBox = new Box(glm::vec2(946, 432), glm::vec2(0, 0), 0, 10, 4, 190,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_rightSideBox2 = new Box(glm::vec2(996, 432), glm::vec2(0, 0), 0, 10, 4, 279,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	// left side
	m_leftSideBox = new Box(glm::vec2(79, 432), glm::vec2(0, 0), 0, 10, 4, 190,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));

	m_leftSideBox2 = new Box(glm::vec2(29, 432), glm::vec2(0, 0), 0, 10, 4, 279,
		glm::vec4(0.6f, 0.25f, 0.01f, 1));


	m_physicsScene->AddActor(m_topLeftBox);
	m_physicsScene->AddActor(m_topRightBox);
	m_physicsScene->AddActor(m_bottomLeftBox);
	m_physicsScene->AddActor(m_bottomRightBox);
	m_physicsScene->AddActor(m_leftSideBox);
	m_physicsScene->AddActor(m_rightSideBox);

	m_physicsScene->AddActor(m_topLeftBox2);
	m_physicsScene->AddActor(m_topRightBox2);
	m_physicsScene->AddActor(m_bottomLeftBox2);
	m_physicsScene->AddActor(m_bottomRightBox2);
	m_physicsScene->AddActor(m_leftSideBox2);
	m_physicsScene->AddActor(m_rightSideBox2);

	m_topLeftBox->SetKinematic(1);
	m_topRightBox->SetKinematic(1);
	m_bottomLeftBox->SetKinematic(1);
	m_bottomRightBox->SetKinematic(1);
	m_leftSideBox->SetKinematic(1);
	m_rightSideBox->SetKinematic(1);

	m_topLeftBox2->SetKinematic(1);
	m_topRightBox2->SetKinematic(1);
	m_bottomLeftBox2->SetKinematic(1);
	m_bottomRightBox2->SetKinematic(1);
	m_leftSideBox2->SetKinematic(1);
	m_rightSideBox2->SetKinematic(1);

	m_topLeftBox->SetElasticity(0.6f);
	m_topRightBox->SetElasticity(0.6f);
	m_bottomLeftBox->SetElasticity(0.6f);
	m_bottomRightBox->SetElasticity(0.6f);
	m_leftSideBox->SetElasticity(0.6f);
	m_rightSideBox->SetElasticity(0.6f);


}

void PhysicsProjectApp::DrawBalls()
{
#pragma region CreateBall

	// White
	whiteBall = new Sphere(glm::vec2(250, 435), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 1, 1, 1));

	// Yellow solid 1
	yellowSolid1 = new Sphere(glm::vec2(700, 435), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 1, 0, 1));

	// Blue solid 2
	blueSolid2 = new Sphere(glm::vec2(774, 421), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0, 0, 1, 1));

	// Red solid 3
	redSolid3 = new Sphere(glm::vec2(800, 463), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0, 0, 1));

	// Purple solid 4
	purpleSolid4 = new Sphere(glm::vec2(750, 463), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0, 1, 1));

	// Orange solid 5
	orangeSolid5 = new Sphere(glm::vec2(800, 406), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0.4, 0, 1));

	// Green solid 6
	greenSolid6 = new Sphere(glm::vec2(750, 407), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0, 1, 0, 1));

	// Maroon solid 7;
	maroonSolid7 = new Sphere(glm::vec2(800, 490), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0.55, 0, 0.1, 1));

	// Black 8
	blackBall8 = new Sphere(glm::vec2(750, 435), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0, 0, 0, 1));

	// Yellow stripe 9
	yellowStripe9 = new Sphere(glm::vec2(800, 435), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 1, 0, 0));

	// Blue stripe 10
	blueStripe10 = new Sphere(glm::vec2(725, 449), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0, 0, 1, 0));

	// Red stripe 11
	redStripe11 = new Sphere(glm::vec2(800, 378), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0, 0, 0));

	// Purple stripe 12
	purpleStripe12 = new Sphere(glm::vec2(774, 392), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0, 1, 0));

	// Orange stripe 13
	orangeStripe13 = new Sphere(glm::vec2(774, 449), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(1, 0.4, 0, 0));

	// Green stripe 14
	greenStripe14 = new Sphere(glm::vec2(774, 478), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0, 1, 0, 0));

	// Maroon stripe 15
	maroonStripe15 = new Sphere(glm::vec2(725, 421), glm::vec2(0, 0), m_ballMass, m_ballRadius, glm::vec4(0.55, 0, 0.1, 0));


#pragma endregion


	AddBallsToList(whiteBall);
	AddBallsToList(blackBall8);
	AddBallsToList(yellowSolid1);
	AddBallsToList(blueSolid2);
	AddBallsToList(redSolid3);
	AddBallsToList(purpleSolid4);
	AddBallsToList(orangeSolid5);
	AddBallsToList(greenSolid6);
	AddBallsToList(maroonSolid7);
	AddBallsToList(yellowStripe9);
	AddBallsToList(blueStripe10);
	AddBallsToList(redStripe11);
	AddBallsToList(purpleStripe12);
	AddBallsToList(orangeStripe13);
	AddBallsToList(greenStripe14);
	AddBallsToList(maroonStripe15);

	AddBallsToScene();
	SetBallsElasticity();
	SetBallsLinearDrag();
	AddSolids();
	AddStripes();

}

void PhysicsProjectApp::DrawPockets()
{
	m_topLeft = new Sphere(glm::vec2(80, 658), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));
	m_topRight = new Sphere(glm::vec2(947, 658), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));

	m_topCentre = new Sphere(glm::vec2(512, 682), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));
	m_bottomCentre = new Sphere(glm::vec2(512, 180), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));

	m_bottomLeft = new Sphere(glm::vec2(80, 207), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));
	m_bottomRight = new Sphere(glm::vec2(947, 207), glm::vec2(0, 0), 10.f, m_pocketSize, glm::vec4(0, 0, 0, 1));





	m_topLeft->SetKinematic(1);
	m_topCentre->SetKinematic(1);
	m_topRight->SetKinematic(1);
	m_bottomLeft->SetKinematic(1);
	m_bottomCentre->SetKinematic(1);
	m_bottomRight->SetKinematic(1);

	m_topLeft->SetTrigger(1);
	m_topCentre->SetTrigger(1);
	m_topRight->SetTrigger(1);
	m_bottomLeft->SetTrigger(1);
	m_bottomCentre->SetTrigger(1);
	m_bottomRight->SetTrigger(1);



	m_physicsScene->AddActor(m_topLeft);
	m_physicsScene->AddActor(m_topCentre);
	m_physicsScene->AddActor(m_topRight);
	m_physicsScene->AddActor(m_bottomLeft);
	m_physicsScene->AddActor(m_bottomCentre);
	m_physicsScene->AddActor(m_bottomRight);

	AddPocketsToList(m_topLeft);
	AddPocketsToList(m_topCentre);
	AddPocketsToList(m_topRight);
	AddPocketsToList(m_bottomLeft);
	AddPocketsToList(m_bottomCentre);
	AddPocketsToList(m_bottomRight);

}

void PhysicsProjectApp::AddPocketsToList(Sphere* a_pocket)
{
	pocketList.push_back(a_pocket);
}

void PhysicsProjectApp::AddBallsToList(Sphere* a_ball)
{
	ballList.push_back(a_ball);
}

void PhysicsProjectApp::AddSolids()
{
	solidBallList.push_back(yellowSolid1);
	solidBallList.push_back(blueSolid2);
	solidBallList.push_back(redSolid3);
	solidBallList.push_back(purpleSolid4);
	solidBallList.push_back(orangeSolid5);
	solidBallList.push_back(greenSolid6);
	solidBallList.push_back(maroonSolid7);

}

void PhysicsProjectApp::AddStripes()
{
	stripeBallList.push_back(yellowStripe9);
	stripeBallList.push_back(blueStripe10);
	stripeBallList.push_back(redStripe11);
	stripeBallList.push_back(purpleStripe12);
	stripeBallList.push_back(orangeStripe13);
	stripeBallList.push_back(greenStripe14);
	stripeBallList.push_back(maroonStripe15);

}

void PhysicsProjectApp::AddBallsToScene()
{
	for (auto pball : ballList)
	{
		m_physicsScene->AddActor(pball);
	}
}

void PhysicsProjectApp::SetBallsElasticity()
{
	for (auto pBall : ballList)
	{
		pBall->SetElasticity(m_pBallElasticity);
	}
}

void PhysicsProjectApp::SetBallsLinearDrag()
{
	for (auto pBall : ballList)
	{
		pBall->SetLinearDrag(m_pBallLinearDrag);
	}
}

#pragma endregion



#pragma region Game Logic

void PhysicsProjectApp::PoolGame(aie::Input* a_input)
{
	if (m_wasFirstShotTaken == true)
	{
		if (HaveAllBallsStopped() == true)
		{
			if (HaveAllBallsStopped() == true && m_ballWasHit == true && m_wasBallSunk == true)
			{
				ChangePlayerCheck();
				//std::cout << "ball was hit and sunk" << std::endl;
				AimAndShoot(a_input);
			}
			else if (HaveAllBallsStopped() == true && m_ballWasHit == true && m_wasBallSunk == false)
			{
				ChangePlayerCheck();
				//std::cout << "ball was hit but not sunk" << std::endl;
				AimAndShoot(a_input);
			}
			if (HaveAllBallsStopped() == true && m_ballWasHit == false)
			{
				ChangePlayerCheck();
				//std::cout << "ball not hit" << std::endl;
				AimAndShoot(a_input);
			}

		}
	}
	else if (m_wasFirstShotTaken == false)
	{
		AimAndShoot(a_input);
	}
	ResetGame(a_input);
}

void PhysicsProjectApp::ChangePlayer()
{
	if (m_isPlayer1Turn == true)
	{
		m_isPlayer2Turn = true;
		m_isPlayer1Turn = false;

		std::cout << "Player 2's turn" << std::endl;
		m_text = "Player 2's turn";

	}
	else if (m_isPlayer2Turn == true)
	{
		m_isPlayer1Turn = true;
		m_isPlayer2Turn = false;

		std::cout << "Player 1's turn" << std::endl;
		m_text = "Player 1's turn";


	}
}

void PhysicsProjectApp::ChangePlayerCheck()
{
	if (m_hasPlayerBeenChecked == false)
	{
		if (m_isFirstBallSunk == true)
		{
			if (m_ballFound == false && m_wasBallSunk == false) // Wrong ball hit and nothing sunk
			{
				ChangePlayer();
			}
			else if (m_ballFound == false && m_wasBallSunk == true) // Wrong ball hit and sunk
			{
				ChangePlayer();
			}
			else if (m_ballFound == true && m_wasBallSunk == false) // Hit right ball but didn't sink
			{
				ChangePlayer();
			}

		}
		else if (m_isFirstBallSunk == false)
		{
			if (m_wasBallSunk == false)
			{
				ChangePlayer();

			}
		}
		m_hasPlayerBeenChecked = true;
	}
}

glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the centre of the screen to (0, 0)
	worldPos.x = getWindowWidth() / 2.f;
	worldPos.y = getWindowHeight() / 2.f;

	// Scale this according to the extents
	//worldPos.x *= 2.f * m_extents / getWindowWidth();
	//worldPos.y *= 2.f * m_extents / (m_aspectRatio * getWindowHeight());



	return worldPos;
}

void PhysicsProjectApp::AimAndShoot(aie::Input* a_input)
{

	if (a_input->isMouseButtonDown(0))
	{
		/*int xScreen, yScreen;
		a_input->getMouseXY(&xScreen, &yScreen);
		worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));*/
		worldPos.x = (float)a_input->getMouseX();
		worldPos.y = (float)a_input->getMouseY();

		aie::Gizmos::add2DCircle(worldPos, 10, 32, glm::vec4(0.3f));
		aie::Gizmos::add2DLine(whiteBall->GetPosition(), worldPos, glm::vec4(1), glm::vec4(1.f, 0.f, 0.f, 1.f));


	}
	if (a_input->wasMouseButtonReleased(0))
	{
		std::cout << "waiting for balls to stop..." << std::endl;
		m_wasFirstShotTaken = true;
		m_wasBallSunk = false;
		m_ballWasHit = false;
		m_hasPlayerBeenChecked = false;
		whiteBall->ApplyForce((worldPos - whiteBall->GetPosition()), glm::vec2(0));

		for (auto pocket : pocketList)
		{
			pocket->triggerEnter = [=](PhysicsObject* other)
			{
				// Sink white ball on the break and nothing else
				if (other == whiteBall)
				{
					WasWhiteBallSunk(other);
				}
			};
			pocket->triggerExit = [=](PhysicsObject* other) {std::cout << "Exited: " << other << std::endl; };
		}
		whiteBall->m_collisionCallback = [=](PhysicsObject* other)
		{

			for (auto pBall : ballList)
			{
				if (m_ballWasHit == false) // ignore collisions with cushions
				{
					if (pBall == other)
						BallHit(other);
				}
			}

		};
	}
}

void PhysicsProjectApp::BallHit(PhysicsObject* other)
{

	if (m_isFirstBallSunk == false)
	{
		BallSunk();
	}
	else if (m_isPlayer1Turn == true && m_player1Solid == true) // player1 and solids
	{
		CheckBallType(other, solidBallList);
	}
	else if (m_isPlayer2Turn == true && m_player2Solid == true) // player2 and solids
	{
		CheckBallType(other, solidBallList);
	}
	else if (m_isPlayer1Turn == true && m_player1Stripe == true) // player1 and stripes
	{
		CheckBallType(other, stripeBallList);
	}
	else if (m_isPlayer2Turn == true && m_player2Stripe == true) // player2 and stripes
	{
		CheckBallType(other, stripeBallList);
	}

	m_ballWasHit = true;

}

void PhysicsProjectApp::CheckBallType(PhysicsObject* other, std::vector<Sphere*> a_list)
{
	m_ballFound = false;
	for (auto pBall : a_list)
	{
		if (a_list.empty() == true)
		{
			m_areAllBallsSunk = true;
			std::cout << "list empty." << std::endl;
			break;
		}
		if (pBall == other)
		{
			BallSunk();
			m_ballFound = true;
			break;
		}
	}
	if (m_ballFound == false)
	{
		BallSunk();
	}
}

void PhysicsProjectApp::BallSunk()
{
	for (auto pocket : pocketList)
	{

		pocket->triggerEnter = [=](PhysicsObject* other)
		{
			WasBlackBallSunk(other);
			if (m_isFirstBallSunk == false)
			{
				// Sink white ball on the break and nothing else
				if (WasWhiteBallSunk(other) == true)
				{
					ChangePlayer();
				}
				else if (other != whiteBall) // Sink first ball on or after break
				{
					m_isFirstBallSunk = true; // first ball has now been sunk
					SetPlayerBallType(other);
					for (auto pBall : ballList)
					{
						if (other == pBall)
						{
							PlaceBallNextToPlayer(pBall);
							m_wasBallSunk = true;
							m_ballFound = true;
							std::cout << "First ball sunk: " << m_wasBallSunk << std::endl;
							std::cout << "Player 1 solids?: " << m_player1Solid << std::endl;
							std::cout << "Player 2 solids?: " << m_player2Solid << std::endl;

						}
					}
				}
			}
			else if (m_isFirstBallSunk == true)
			{
				if (other != whiteBall)
				{
					for (auto pBall : ballList)
					{
						if (other == pBall)
						{
							PlaceBallNextToPlayer(pBall);
							m_wasBallSunk = true;

							std::cout << "ball sunk: " << m_wasBallSunk << std::endl;

						}
					}
				}
				else if (WasWhiteBallSunk(other) == true)
				{
					ChangePlayer();
				}

			}
		};

		pocket->triggerExit = [=](PhysicsObject* other) {std::cout << "Exited: " << other << std::endl; };
	}

}

void PhysicsProjectApp::PlaceBallNextToPlayer(Sphere* a_ball)
{
	a_ball->SetKinematic(1);
	if (m_isPlayer1Turn == true)
	{
		a_ball->SetPosition({ m_p1sunkPosX , m_p1sunkPosY });
		m_p1sunkPosX += 50.f;
	}
	else if (m_isPlayer2Turn == true)
	{
		a_ball->SetPosition({ m_p2sunkPosX , m_p2sunkPosY });
		m_p2sunkPosX += 50.f;
	}
}

bool PhysicsProjectApp::HaveAllBallsStopped()
{
	for (auto pball : ballList)
	{
		if (glm::sqrt(glm::abs(pball->GetVelocity().x) + glm::abs(pball->GetVelocity().y)) < 0.9f)
		{
			pball->SetVelocity({ 0, 0 });
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool PhysicsProjectApp::WasWhiteBallSunk(PhysicsObject* other)
{
	if (other == whiteBall)
	{
		whiteBall->SetPosition({ 250, 435 });
		whiteBall->SetVelocity({ 0, 0 });
		std::cout << "White ball sunk" << std::endl;
		return true;
	}
	return false;
}

bool PhysicsProjectApp::WasBlackBallSunk(PhysicsObject* other)
{
	if (other == blackBall8)
	{
		if (m_areAllBallsSunk == true)
		{
			std::cout << "You win" << std::endl;
		}
		else if (m_areAllBallsSunk == false)
		{
			std::cout << "You lose" << std::endl;
		}
		return true;
	}
	return false;
}

void PhysicsProjectApp::SetPlayerBallType(PhysicsObject* other)
{
	for (auto pBall : solidBallList)
	{
		if (pBall == other)
		{
			m_solidFound = true;
		}
	}
	for (auto pBall : stripeBallList)
	{
		if (pBall == other)
		{
			m_stripeFound = true;
		}
	}

	if (m_isPlayer1Turn == true && m_solidFound == true) // player 1 solids
	{
		m_player1Solid = true;
		m_player2Stripe = true;
	}
	else if (m_isPlayer1Turn == true && m_stripeFound == true) // player 1 stripes
	{
		m_player1Stripe = true;
		m_player2Solid = true;
	}
	else if (m_isPlayer2Turn == true && m_solidFound == true) // player 2 solids
	{
		m_player2Solid = true;
		m_player1Stripe = true;
	}
	else if (m_isPlayer2Turn == true && m_stripeFound == true) // player 2 stripes
	{
		m_player1Solid = true;
		m_player2Stripe = true;
	}

}

void PhysicsProjectApp::ResetGame(aie::Input* a_input)
{
	if (a_input->wasKeyPressed(82))
	{
		for (auto pBall : ballList)
		{
			if (pBall != nullptr)
			{
				ballList.pop_back();
				m_physicsScene->RemoveActor(pBall);

				delete pBall;
			}
		}
		m_isPlayer1Turn = true;
		m_isPlayer2Turn = false;
		m_isFirstBallSunk = false;
		m_areAllBallsSunk = false;
		m_wasBallSunk = false;
		m_ballWasHit = false;
		m_player1Solid = false;
		m_player2Solid = false;
		m_player1Stripe = false;
		m_player2Stripe = false;
		m_solidFound = false;
		m_stripeFound = false;
		m_ballFound = false;
		m_wasFirstShotTaken = false;
		m_hasPlayerBeenChecked = false;
		m_text = "Player 1's turn";
		DrawBalls();

	}
}

#pragma endregion

