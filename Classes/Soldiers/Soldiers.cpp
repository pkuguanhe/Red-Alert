﻿#include"Soldiers.h"

EventListenerTouchOneByOne * Soldiers::touchSoldierListener;
EventDispatcher * Soldiers::eventDispatcher;

Soldiers::Soldiers(SoldierTypes soldierType)
{
	this->soldiertype = soldierType;
	this->health = 0;
	this->price = 0;
	this->ifselect = SELECT_OFF;
	this->maxHealth = 0;
}

Soldiers * Soldiers::createWithSoldierTypes(SoldierTypes soldierType)
{
	Soldiers *soldier = new Soldiers(soldierType);
	const char *soldierName = "soldiername";
	switch (soldierType)
	{
	case START_MINER:
	{
		soldierName = MINER1;
		soldier->health = MINER_HEALTH;
		soldier->price = MINER_PRICE;
		soldier->ifselect = SELECT_OFF;
		soldier->maxHealth = MINER_HEALTH;
		soldier->speed = MINER_SPEED;
		break;
	}
	case START_POLICEMAN:
	{
		soldierName = POLICEMAN1;
		soldier->health = POLICEMAN_HEALTH;
		soldier->price = POLICEMAN_PRICE;
		soldier->ifselect = SELECT_OFF;
		soldier->maxHealth = POLICEMAN_HEALTH;
		soldier->speed = POLICEMAN_SPEED;
		break;
	}
	case START_WARRIOR:
	{
		soldierName = WARRIOR1;
		soldier->health = WARRIOR_HEALTH;
		soldier->price = WARRIOR_PRICE;
		soldier->ifselect = SELECT_OFF;
		soldier->maxHealth = WARRIOR_HEALTH;
		soldier->speed = WARRIOR_SPEED;
		break;
	}
	case START_TANK:
	{
		soldierName = TANK1;
		soldier->health = TANK_HEALTH;
		soldier->price = TANK_PRICE;
		soldier->ifselect = SELECT_OFF;
		soldier->maxHealth = TANK_HEALTH;
		soldier->speed = TANK_SPEED;
	}


	}
	if (soldier && soldier->initWithFile(soldierName))
	{
		soldier->autorelease();
		touchSoldierListener = EventListenerTouchOneByOne::create();
		//touchSoldierListener->setSwallowTouches(true);
		touchSoldierListener->onTouchBegan = [&](Touch *touch, Event *event)
		{
			log("soldier");
			/*auto target = dynamic_cast<Soldiers *>(event->getCurrentTarget());
			if (!target->getifSelect())
			{
			return false;
			}*/
			return true;
		};
		touchSoldierListener->onTouchEnded = [](Touch *touch, Event *event)
		{
			auto origin = Director::getInstance()->getVisibleOrigin();
			//auto target = dynamic_cast<Soldiers *>(event->getCurrentTarget());
			Rect rect = GameScene::getSelectRect();
			for (auto &sprite : GameScene::gettiledMap()->getChildren())
			{
				/*if (!rect.containsPoint(sprite->getPosition()))
				{
				continue;
				}*/
				if (sprite->getTag() == GameSceneNodeTagSoldier)
				{
					auto target = dynamic_cast<Soldiers *>(sprite);
					if (target->getifSelect())
					{
						auto pos1 = GameScene::gettiledMap()->convertTouchToNodeSpace(touch);
						auto pos2 = target->getPosition();
						float distance = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
						MoveTo *soldierMove = MoveTo::create(distance/target->speed, pos1);
						target->runAction(soldierMove);
						target->setifSelect(SELECT_OFF);
					}
				}
			}
		};

		eventDispatcher = Director::getInstance()->getEventDispatcher();
		eventDispatcher->addEventListenerWithSceneGraphPriority(touchSoldierListener, soldier);

		auto body = PhysicsBody::createBox((soldier->getContentSize()));
		body->setCategoryBitmask(0x02);
		body->setContactTestBitmask(0x02);
		body->setCollisionBitmask(0x04);
		soldier->setPhysicsBody(body);

		return soldier;
	}
	CC_SAFE_DELETE(soldier);
	return nullptr;
}

Apoint Soldiers::turnToApoint(Vec2 vecPoint)
{
	int x = vecPoint.x / 16;
	int y = ((100 * 16) - vecPoint.y) / 16;
	return Apoint(x, y);
}

void Soldiers::createBar()
{
	hpBar = Bar::create();
	float bar_width = this->getContentSize().width;
	float bar_height = this->getContentSize().height;
	hpBar->setLength(bar_width);
	hpBar->setHeight(2);
	hpBar->setVisible(false);
	addChild(hpBar, 20);
	hpBar->setPosition(Point(0, bar_height + 5));
}

void Soldiers::displayHpBar()
{
	if (hpBar)
	{
		hpBar->schedule(schedule_selector(Bar::update));
		hpBar->setVisible(true);
	}
}

void Soldiers::hideHpBar()
{
	if (hpBar)
	{
		hpBar->unschedule(schedule_selector(Bar::update));
		hpBar->setVisible(false);
	}
}
