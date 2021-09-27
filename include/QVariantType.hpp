#pragma once
#include <QVariant>

#include "type_traits"

using QVariantType = std::underlying_type_t<QVariant::Type>;
