#pragma once
class BaseObject : public enable_shared_from_this<BaseObject>
{
public:
	BaseObject();
	virtual ~BaseObject();

public:
	Protocol::ObjectInfo* GetObjectInfo() { return objectInfo; }
	RoomRef GetRoom() { return room.load().lock();  }
	shared_ptr<class BoxCollider> GetBoxCollider() { return boxCollider; }


public:
	virtual void Update() abstract;

public:
	atomic<weak_ptr<Room>> room;

public:
	//int32 attackSequence = 0;
	//bool canAttack = true;

protected:
	Protocol::ObjectInfo* objectInfo;
	shared_ptr<class BoxCollider> boxCollider;

	

};

