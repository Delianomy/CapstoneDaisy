#ifndef SCENE_H
#define SCENE_H
#include "Actor.h"

union SDL_Event; /// Forward declaration for HandleEvents

struct ViewPort {
	int x;
	int y;
	int width;
	int height;

	inline float GetAspectRatio() {
		return static_cast<float>(width) / static_cast<float>(height);
	}
	inline operator const int* () const {
		return static_cast<const int*>(&x);
	}
	inline operator int* () {
		return static_cast<int*>(&x);
	}
};

class Scene{
protected:
	std::vector<Ref<Actor>> actors;
	std::vector<Ref<Actor>> opaqueActors;
	std::vector<Ref<Actor>> transparentActors;
	ViewPort viewport;
public:	
	virtual ~Scene() {}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	
	template<typename ActorTemplate>
	void AddActor(Ref<ActorTemplate> actor_) {
		actors.push_back(actor_);
	}

	template<typename ActorTemplate>
	void AddTransparentActor(Ref<ActorTemplate> actor_) {
		transparentActors.push_back(actor_);
	}


	template<typename ActorTemplate>
	void AddOpaqueActor(Ref<ActorTemplate> actor_) {
		opaqueActors.push_back(actor_);
	}

	template<typename ActorTemplate, typename ... Args>
	void AddActor(Args&& ... args_) {
		actors.push_back(std::make_shared<ActorTemplate>(std::forward<Args>(args_)...));
	}

};
#endif