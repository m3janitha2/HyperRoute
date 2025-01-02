#include <gtest/gtest.h>

#include <NewsTracker.h>
#include <Application.h>

class NewsTrackerTest : public ::testing::Test
{
public:
	NewsTrackerTest() = default;
	~NewsTrackerTest() override = default;

	NewsTrackerTest(const NewsTrackerTest&) = delete;
	NewsTrackerTest(NewsTrackerTest&&) = delete;
	NewsTrackerTest& operator= (const NewsTrackerTest&) = delete;
	NewsTrackerTest& operator= (NewsTrackerTest&&) = delete;
	
	NewsTracker nt_;
};

void CheckCurrentDataUsage(TopicSources topicSources, TopicSources expected_values)
{
	EXPECT_EQ(topicSources.size(), expected_values.size());

	for (auto& expected : expected_values)
	{
		auto it = std::find_if(topicSources.begin(), topicSources.end(), [&expected](TopicSource& topicSource)
		{
			return topicSource.sourceId == expected.sourceId
				&& topicSource.topicId == expected.topicId;
		});

		EXPECT_TRUE(it != topicSources.end());
	}
}

// topics:	from 1		to 99
// sources: from 100	to 999
// apps:	from 1000	to 9999

// check each value is present in the other vector
void CompareTwoVectors(std::vector<int> values, std::vector<int> expected_values)
{
	EXPECT_EQ(values.size(), expected_values.size());

	for (auto expected : expected_values)
	{
		EXPECT_TRUE(std::find(values.begin(), values.end(), expected) != values.end());
	}
}

void CheckCurrentSources(std::vector<int> sources, std::vector<int> expected_values)
{
	CompareTwoVectors(sources, expected_values);
}

void CheckCurrentTopics(std::vector<int> topics, std::vector<int> expected_values)
{
	CompareTwoVectors(topics, expected_values);
}

void CheckTopicUsage(std::vector<int> topics, std::vector<int> expected_values)
{
	CompareTwoVectors(topics, expected_values);
}

void CheckAnyOf(int expected, std::vector<int> expected_values)
{
	EXPECT_TRUE(std::find(expected_values.begin(), expected_values.end(), expected) != expected_values.end());
}

void CheckUnusedPublishers(std::vector<int> publishers, std::vector<int> expected_values)
{
	CompareTwoVectors(publishers, expected_values);
}

TEST_F(NewsTrackerTest, 0_Applications)
{
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1001), {});

	CheckCurrentSources(nt_.GetCurrentSources(1), {});

	CheckCurrentTopics(nt_.GetCurrentTopics(101), {});

	CheckTopicUsage(nt_.GetTopicUsage(1), {});
	CheckTopicUsage(nt_.GetTopicUsage(2), {});

	EXPECT_EQ(nt_.GetLongestRepublishChain(), 0);

	EXPECT_EQ(nt_.GetMostSubscribedTopic(), 0);

	EXPECT_EQ(nt_.DistanceFromSource(1001, 1), 0);
	EXPECT_EQ(nt_.DistanceFromSource(1001, 2), 0);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 3), 0);

	CheckUnusedPublishers(nt_.FindUnusedPublishers(1), {});
	CheckUnusedPublishers(nt_.FindUnusedPublishers(2), {});
	CheckUnusedPublishers(nt_.FindUnusedPublishers(3), {});
}

TEST_F(NewsTrackerTest, 1_Application)
{
	Application app1(1001, nt_,
		{ {1,101}, {2,101}, {3,102} },
		{});
	
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1001),
		{ {1,101}, {2,101}, {3,102} });

	CheckCurrentSources(nt_.GetCurrentSources(1), { 101 });
	CheckCurrentSources(nt_.GetCurrentSources(2), { 101 });
	CheckCurrentSources(nt_.GetCurrentSources(3), { 102 });

	CheckCurrentTopics(nt_.GetCurrentTopics(101), { 1,2 });
	CheckCurrentTopics(nt_.GetCurrentTopics(102), { 3 });

	CheckTopicUsage(nt_.GetTopicUsage(1), { 1001 });
	CheckTopicUsage(nt_.GetTopicUsage(2), { 1001 });
	CheckTopicUsage(nt_.GetTopicUsage(3), { 1001 });

	EXPECT_EQ(nt_.GetLongestRepublishChain(), 1);

	CheckAnyOf(nt_.GetMostSubscribedTopic(), { 1, 2, 3 });

	EXPECT_EQ(nt_.DistanceFromSource(1001, 1),1);
	EXPECT_EQ(nt_.DistanceFromSource(1001, 2), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1001, 3), 1);

	CheckUnusedPublishers(nt_.FindUnusedPublishers(1), {});
	CheckUnusedPublishers(nt_.FindUnusedPublishers(2), {});
	CheckUnusedPublishers(nt_.FindUnusedPublishers(3), {});
}

TEST_F(NewsTrackerTest, 5_Applications)
{
	Application app1(1001, nt_,
		{ {1,101}, {2,101}, {3,102} },
		{});

	Application app2(1002, nt_,
		{ {4,103}, {5,103}, {6,102} },
		{ {1,1001}, {2,1001}, {3,1001}, { 7, 1004 } });

	Application app3(1003, nt_,
		{ {3,104}, {5,103} },
		{ {1,1002}, {2,1001} });

	Application app4(1004, nt_,
		{ {2,104}, {7,104} },
		{ {1,1003}, });

	Application app5(1005, nt_,
		{},
		{ {2, 1001},  {7,1002}, });


	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1001),
		{ {1,101}, {2,101}, {3,102} });
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1002),
		{ {1,101}, {2,101}, {3,102}, {4,103}, {5,103}, {6,102}, {7,104} });
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1003),
		{ {1,101}, {2,101}, {3,104}, {5,103} });
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1004),
		{ {1,101}, {2,104}, {7,104} });
	CheckCurrentDataUsage(nt_.GetCurrentDataUsage(1005),
		{ {2,101}, {7,104} });


	CheckCurrentSources(nt_.GetCurrentSources(1), { 101 });
	CheckCurrentSources(nt_.GetCurrentSources(2), { 101, 104 });
	CheckCurrentSources(nt_.GetCurrentSources(3), { 102, 104 });
	CheckCurrentSources(nt_.GetCurrentSources(4), { 103 });
	CheckCurrentSources(nt_.GetCurrentSources(5), { 103 });
	CheckCurrentSources(nt_.GetCurrentSources(6), { 102 });
	CheckCurrentSources(nt_.GetCurrentSources(7), { 104 });


	CheckCurrentTopics(nt_.GetCurrentTopics(101), { 1,2 });
	CheckCurrentTopics(nt_.GetCurrentTopics(102), { 3,6 });
	CheckCurrentTopics(nt_.GetCurrentTopics(103), { 4,5 });
	CheckCurrentTopics(nt_.GetCurrentTopics(104), { 2,3,7 });


	CheckTopicUsage(nt_.GetTopicUsage(1), { 1001,1002, 1003,1004 });
	CheckTopicUsage(nt_.GetTopicUsage(2), { 1001, 1002,1003,1004, 1005 });
	CheckTopicUsage(nt_.GetTopicUsage(3), { 1001, 1002, 1003 });
	CheckTopicUsage(nt_.GetTopicUsage(4), { 1002 });
	CheckTopicUsage(nt_.GetTopicUsage(5), { 1002, 1003 });
	CheckTopicUsage(nt_.GetTopicUsage(6), { 1002 });
	CheckTopicUsage(nt_.GetTopicUsage(7), { 1002, 1004, 1005 });


	EXPECT_EQ(nt_.GetLongestRepublishChain(), 4);


	EXPECT_EQ(nt_.GetMostSubscribedTopic(), 2);


	EXPECT_EQ(nt_.DistanceFromSource(1001, 1), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1001, 2), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1001, 3), 1);

	EXPECT_EQ(nt_.DistanceFromSource(1002, 1), 2);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 2), 2);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 3), 2);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 4), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 5), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 6), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1002, 7), 2);

	EXPECT_EQ(nt_.DistanceFromSource(1003, 1), 3);
	EXPECT_EQ(nt_.DistanceFromSource(1003, 2), 2);
	EXPECT_EQ(nt_.DistanceFromSource(1003, 3), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1003, 5), 1);

	EXPECT_EQ(nt_.DistanceFromSource(1004, 1), 4);
	EXPECT_EQ(nt_.DistanceFromSource(1004, 2), 1);
	EXPECT_EQ(nt_.DistanceFromSource(1004, 7), 1);

	EXPECT_EQ(nt_.DistanceFromSource(1005, 2), 2);
	EXPECT_EQ(nt_.DistanceFromSource(1005, 7), 3);


	CheckUnusedPublishers(nt_.FindUnusedPublishers(1), { 1005 });
	CheckUnusedPublishers(nt_.FindUnusedPublishers(2), {});
	CheckUnusedPublishers(nt_.FindUnusedPublishers(3), { 1004,1005 });
	CheckUnusedPublishers(nt_.FindUnusedPublishers(4), { 1001, 1003, 1004, 1005 });
	CheckUnusedPublishers(nt_.FindUnusedPublishers(5), { 1001, 1004, 1005 });
	CheckUnusedPublishers(nt_.FindUnusedPublishers(6), { 1001, 1003, 1004, 1005 });
	CheckUnusedPublishers(nt_.FindUnusedPublishers(7), { 1001, 1003 });
}

int main(int argc, char** argv) 
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}