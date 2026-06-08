#pragma once
#include <vector>
#include <optional>
#include "data/Subject.hpp"

std::optional<TimeBlock> parseTime(const Day& day, const std::string& str);

bool haveCollision(TimeBlock block1, TimeBlock block2);

bool haveCollision(Subject sub1, Subject sub2);
