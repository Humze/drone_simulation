#ifndef ENTITY_DECORATOR_H_
#define ENTITY_DECORATOR_H_

#include <concepts>

#include "IEntity.h"

/**
 * @brief IEntityDecorator is a decorator that decorates an IEntity
 */
template <std::derived_from<IEntity> T = IEntity>
class IEntityDecorator : public T {
 public:
  /**
   * @brief Constructor for IEntityDecorator
   * @param e The entity to decorate
   */
  IEntityDecorator(T* e) : T(e->getDetails()), sub(e) {}
  /**
   * @brief Destructor for IEntityDecorator
   */
  virtual ~IEntityDecorator() {
    if (sub) delete sub;
  }
  /**
   * @brief Link the model to the entity
   * @param model The model to link to
   */
  virtual void linkModel(SimulationModel* model) {
    return sub->linkModel(model);
  }
  /**
   * @brief Get the id of the entity
   * @return The id of the entity
   */
  virtual int getId() const { return sub->getId(); }
  /**
   * @brief Get the position of the entity
   * @return The position of the entity
   */
  virtual Vector3 getPosition() const { return sub->getPosition(); }
  /**
   * @brief Get the direction of the entity
   * @return The direction of the entity
   */
  virtual Vector3 getDirection() const { return sub->getDirection(); }
  /**
   * @brief Get the details of the entity
   * @return The details of the entity
   */
  virtual const JsonObject& getDetails() const { return sub->getDetails(); }
  /**
   * @brief Get the color of the entity
   * @return The color of the entity
   */
  virtual std::string getColor() const { return sub->getColor(); }
  /**
   * @brief Get the name of the entity
   * @return The name of the entity
   */
  virtual std::string getName() const { return sub->getName(); }
  /**
   * @brief Get the speed of the entity
   * @return The speed of the entity
   */
  virtual double getSpeed() const { return sub->getSpeed(); }
  /**
   * @brief Set the position of the entity
   * @param pos_ The position to set the entity to
   */
  virtual void setPosition(Vector3 pos_) { return sub->setPosition(pos_); }
  /**
   * @brief Set the direction of the entity
   * @param dir_ The direction to set the entity to
   */
  virtual void setDirection(Vector3 dir_) { return sub->setDirection(dir_); }
  /**
   * @brief Set the color of the entity
   * @param col_ The color to set the entity to
   */
  virtual void setColor(std::string col_) { return sub->setColor(col_); }
  /**
   * @brief Rotate the entity
   * @param angle The angle to rotate the entity by
   */
  virtual void rotate(double angle) { return sub->rotate(angle); }
  /**
   * @brief Update the entity
   * @param dt The time step
   */
  virtual void update(double dt) { return sub->update(dt); }
  /**
   * @brief Add an observer to the entity
   * @param o The observer to add
   */
  virtual void addObserver(const IObserver* o) { return sub->addObserver(o); };
  /**
   * @brief Remove an observer from the entity
   * @param o The observer to remove
   */
  virtual void removeObserver(const IObserver* o) {
    return sub->removeObserver(o);
  };
  /**
   * @brief Notify the observers of the entity
   * @param message The message to notify the observers with
   */
  virtual void notifyObservers(const std::string& message) {
    return sub->notifyObservers(message);
  };

 protected:
  T* sub = nullptr;
};

#endif