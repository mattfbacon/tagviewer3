#pragma once

template <typename Data>
class DataBackedModel {
public:
	virtual void load(Data const& new_data) = 0;
	virtual Data const& save() const = 0;
};
