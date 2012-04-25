//This file implements properties for our kernel
//from codeguru.com/cpp/cpp/cpp_mfc/article.php/c4031/Implementing-a-property-in-C.htm


#ifndef __PROPERTY_H
#define __PROPERTY_H

#define PROP_READ_ONLY 1
#define PROP_WRITE_ONLY 2
#define PROP_READ_WRITE 3

template<typename Container, typename ValueType, int nPropType>
class property
{
	public:
		property()
		{
			cObject = NULL;
			Set = NULL;
			Get = NULL;
			bareValue = NULL;
		}

		property(Container* cObject, void (Container::*Set)(ValueType value), ValueType (Container::*Get)())
		{
			this->cObject = cObject;
			this->Set = Set;
			this->Get = Get;
			bareValue = NULL;
		}

		property(Container* cObject, ValueType* bareValue)
		{
			this->cObject = cObject;
			this->Set = NULL;
			this->Get = NULL;
			this->bareValue = bareValue;
		}

		void setContainer(Container* cObject)
		{
			//set pointer to object that contains this property
			this->cObject = cObject;
			return;
		}

		void setter(void (Container::*pSet)(ValueType value))
		{
			//set the setter function;
			//The argument is a pointer to a function that accepts a value
			this->Set = pSet;
			return;
		}

		void getter(ValueType (Container::*pGet)())
		{
			//Set pointer to getter function
			this->Get = pGet;
			return;
		}
			

		//property magic happens by overloading =
		ValueType operator=(const ValueType& value)
		{
			if (nPropType == PROP_READ_ONLY)
				; //might want to throw something here later
			if (Set != NULL)
			{

				(cObject->*Set)(value);
				return value;
			}
			else
			{
				*bareValue = value;
				return *bareValue;
			}
		}

		//We need to be able to cast the property itself to the type of the value in it
		operator ValueType()
		{
			if (nPropType == PROP_WRITE_ONLY)
				; //thorw exception alter
			if (Get != NULL)
				return (cObject->*Get)();
			else
				return *bareValue;
		}

	private:
		Container* cObject; //pointer to the container object
		void (Container::*Set)(ValueType value);
		ValueType (Container::*Get)();
		ValueType* bareValue;
};

#endif
