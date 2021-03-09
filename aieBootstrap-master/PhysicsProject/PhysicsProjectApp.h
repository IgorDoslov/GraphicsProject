#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Sphere.h"
#include "PhysicsScene.h"
#include "Input.h"
#include <vector>
#include <map>
#include <string>
#include "Box.h"

class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

	void AimAndShoot(aie::Input* a_input);

	void AddBallsToList(Sphere* a_ball);
	void AddPocketsToList(Sphere* a_pocket);

	void AddSolids();
	void AddStripes();
	void AddBallsToScene();
	void SetBallsElasticity();
	void SetBallsLinearDrag();

	bool HaveAllBallsStopped();

	void BallSunk();
	bool WasWhiteBallSunk(PhysicsObject* other);
	bool WasBlackBallSunk(PhysicsObject* other);
	void BallHit(PhysicsObject* other);
	void CheckBallType(PhysicsObject* other, std::vector<Sphere*> a_list);
	void SetPlayerBallType(PhysicsObject* other);
	void ChangePlayer();
	void ChangePlayerCheck();
	void PlaceBallNextToPlayer(Sphere* a_ball);
	void PoolGame(aie::Input* a_input);

	void DrawTable();
	void DrawBalls();
	void DrawPockets();
	void ResetGame(aie::Input* a_input);

protected:

	aie::Renderer2D* m_2dRenderer = nullptr;
	aie::Font* m_font = nullptr;
	aie::Font* m_ballFont = nullptr;
	Sphere* m_ball = nullptr;
	PhysicsScene* m_physicsScene = nullptr;
	std::string m_text = "Player 1's turn";
	
	
	//const float m_aspectRatio = 16.f / 9.f;
	//const float m_extents = 100.f;
	

#pragma region Pool Balls

	float m_ballRadius = 14.f;
	float m_ballMass = 1.f;
	float m_pBallElasticity = 0.9f;
	float m_pBallLinearDrag = 0.7f;
	
	Sphere* whiteBall = nullptr;
	Sphere* blackBall8 = nullptr;
	Sphere* yellowSolid1 = nullptr;
	Sphere* blueSolid2 = nullptr;
	Sphere* redSolid3 = nullptr;
	Sphere* purpleSolid4 = nullptr;
	Sphere* orangeSolid5 = nullptr;
	Sphere* greenSolid6 = nullptr;
	Sphere* maroonSolid7 = nullptr;
	Sphere* yellowStripe9 = nullptr;
	Sphere* blueStripe10 = nullptr;
	Sphere* redStripe11 = nullptr;
	Sphere* purpleStripe12 = nullptr;
	Sphere* orangeStripe13 = nullptr;
	Sphere* greenStripe14 = nullptr;
	Sphere* maroonStripe15 = nullptr;
	

#pragma endregion

#pragma region Pockets

	float m_pocketSize = 22.f;

	Sphere* m_topLeft = nullptr;
	Sphere* m_topCentre = nullptr;
	Sphere* m_topRight = nullptr;
	Sphere* m_bottomLeft = nullptr;
	Sphere* m_bottomCentre = nullptr;
	Sphere* m_bottomRight = nullptr;

#pragma endregion


#pragma region Table


	Box* m_topLeftBox = nullptr;
	Box* m_topRightBox = nullptr;
	Box* m_bottomLeftBox = nullptr;
	Box* m_bottomRightBox = nullptr;
	Box* m_leftSideBox = nullptr;
	Box* m_rightSideBox = nullptr;
	
	Box* m_topLeftBox2 = nullptr;
	Box* m_topRightBox2	= nullptr;
	Box* m_bottomLeftBox2 = nullptr;
	Box* m_bottomRightBox2 = nullptr;
	Box* m_leftSideBox2 = nullptr;
	Box* m_rightSideBox2 = nullptr;

#pragma endregion

	float m_p1sunkPosX = 200.f;
	float m_p1sunkPosY = 120.f;
	float m_p2sunkPosX = 200.f;
	float m_p2sunkPosY = 70.f;

	bool m_isPlayer1Turn = true;
	bool m_isPlayer2Turn = false;
	bool m_isFirstBallSunk = false;
	bool m_areAllBallsSunk = false;
	bool m_wasBallSunk = false;
	bool m_ballWasHit = false;
	bool m_player1Solid = false;
	bool m_player2Solid = false;
	bool m_player1Stripe = false;
	bool m_player2Stripe = false;
	bool m_solidFound = false;
	bool m_stripeFound = false;
	bool m_ballFound = false;
	bool m_wasFirstShotTaken = false;
	bool m_hasPlayerBeenChecked = false;

	//int m_p1ShotCount = 1;
	//int m_p2ShotCount = 1;


	glm::vec2 worldPos = glm::vec2(0);

	std::vector<Sphere*> ballList;
	std::vector<Sphere*> solidBallList;
	std::vector<Sphere*> stripeBallList;
	std::vector<Sphere*> pocketList;
};